YUI.add('moodle-core-tooltip', function (Y, NAME) {

/**
 * Provides the base tooltip class.
 *
 * @module moodle-core-tooltip
 */

/**
 * A base class for a tooltip.
 *
 * @param {Object} config Object literal specifying tooltip configuration properties.
 * @class M.core.tooltip
 * @constructor
 * @extends M.core.dialogue
 */
function TOOLTIP(config) {
    if (!config) {
        config = {};
    }

    // Override the default options provided by the parent class.
    if (typeof config.draggable === 'undefined') {
        config.draggable = true;
    }

    if (typeof config.constrain === 'undefined') {
        config.constrain = true;
    }

    if (typeof config.lightbox === 'undefined') {
        config.lightbox = false;
    }

    TOOLTIP.superclass.constructor.apply(this, [config]);
}

var SELECTORS = {
        CLOSEBUTTON: '.closebutton'
    },

    CSS = {
        PANELTEXT: 'tooltiptext'
    },
    RESOURCES = {
        WAITICON: {
            pix: 'i/loading_small',
            component: 'moodle'
        }
    },
    ATTRS = {};

/**
 * Static property provides a string to identify the JavaScript class.
 *
 * @property NAME
 * @type String
 * @static
 */
TOOLTIP.NAME = 'moodle-core-tooltip';

/**
 * Static property used to define the CSS prefix applied to tooltip dialogues.
 *
 * @property CSS_PREFIX
 * @type String
 * @static
 */
TOOLTIP.CSS_PREFIX = 'moodle-dialogue';

/**
 * Static property used to define the default attribute configuration for the Tooltip.
 *
 * @property ATTRS
 * @type String
 * @static
 */
TOOLTIP.ATTRS = ATTRS;

/**
 * The initial value of the header region before the content finishes loading.
 *
 * @attribute initialheadertext
 * @type String
 * @default ''
 * @writeOnce
 */
ATTRS.initialheadertext = {
    value: ''
};

/**
  * The initial value of the body region before the content finishes loading.
  *
  * The supplid string will be wrapped in a div with the CSS.PANELTEXT class and a standard Moodle spinner
  * appended.
  *
  * @attribute initialbodytext
  * @type String
  * @default ''
  * @writeOnce
  */
ATTRS.initialbodytext = {
    value: '',
    setter: function(content) {
        var parentnode,
            spinner;
        parentnode = Y.Node.create('<div />')
            .addClass(CSS.PANELTEXT);

        spinner = Y.Node.create('<img />')
            .setAttribute('src', M.util.image_url(RESOURCES.WAITICON.pix, RESOURCES.WAITICON.component))
            .addClass('spinner');

        if (content) {
            // If we have been provided with content, add it to the parent and make
            // the spinner appear correctly inline
            parentnode.set('text', content);
            spinner.addClass('iconsmall');
        } else {
            // If there is no loading message, just make the parent node a lightbox
            parentnode.addClass('content-lightbox');
        }

        parentnode.append(spinner);
        return parentnode;
    }
};

/**
 * The initial value of the footer region before the content finishes loading.
 *
 * If a value is supplied, it will be wrapped in a <div> first.
 *
 * @attribute initialfootertext
 * @type String
 * @default ''
 * @writeOnce
 */
ATTRS.initialfootertext = {
    value: null,
    setter: function(content) {
        if (content) {
            return Y.Node.create('<div />')
                .set('text', content);
        }
    }
};

/**
 * The function which handles setting the content of the title region.
 * The specified function will be called with a context of the tooltip instance.
 *
 * The default function will simply set the value of the title to object.heading as returned by the AJAX call.
 *
 * @attribute headerhandler
 * @type Function|String|null
 * @default set_header_content
 */
ATTRS.headerhandler = {
    value: 'set_header_content'
};

/**
 * The function which handles setting the content of the body region.
 * The specified function will be called with a context of the tooltip instance.
 *
 * The default function will simply set the value of the body area to a div containing object.text as returned
 * by the AJAX call.
 *
 * @attribute bodyhandler
 * @type Function|String|null
 * @default set_body_content
 */
ATTRS.bodyhandler = {
    value: 'set_body_content'
};

/**
 * The function which handles setting the content of the footer region.
 * The specified function will be called with a context of the tooltip instance.
 *
 * By default, the footer is not set.
 *
 * @attribute footerhandler
 * @type Function|String|null
 * @default null
 */
ATTRS.footerhandler = {
    value: null
};

/**
 * The function which handles modifying the URL that was clicked on.
 *
 * The default function rewrites '.php' to '_ajax.php'.
 *
 * @attribute urlmodifier
 * @type Function|String|null
 * @default null
 */
ATTRS.urlmodifier = {
    value: null
};

/**
 * Set the Y.Cache object to use.
 *
 * By default a new Y.Cache object will be created for each instance of the tooltip.
 *
 * In certain situations, where multiple tooltips may share the same cache, it may be preferable to
 * seed this cache from the calling method.
 *
 * @attribute textcache
 * @type Y.Cache|null
 * @default null
 */
ATTRS.textcache = {
    value: null
};

/**
 * Set the default size of the Y.Cache object.
 *
 * This is only used if no textcache is specified.
 *
 * @attribute textcachesize
 * @type Number
 * @default 10
 */
ATTRS.textcachesize = {
    value: 10
};

Y.extend(TOOLTIP, M.core.dialogue, {
    // The bounding box.
    bb: null,

    // Any event listeners we may need to cancel later.
    listenevents: [],

    // Cache of objects we've already retrieved.
    textcache: null,

    // The align position. This differs for RTL languages so we calculate once and store.
    alignpoints: [
        Y.WidgetPositionAlign.TL,
        Y.WidgetPositionAlign.RC
    ],

    initializer: function() {
        // Set the initial values for the handlers.
        // These cannot be set in the attributes section as context isn't present at that time.
        if (!this.get('headerhandler')) {
            this.set('headerhandler', this.set_header_content);
        }
        if (!this.get('bodyhandler')) {
            this.set('bodyhandler', this.set_body_content);
        }
        if (!this.get('footerhandler')) {
            this.set('footerhandler', function() {});
        }
        if (!this.get('urlmodifier')) {
            this.set('urlmodifier', this.modify_url);
        }

        // Set up the dialogue with initial content.
        this.setAttrs({
            headerContent: this.get('initialheadertext'),
            bodyContent: this.get('initialbodytext'),
            footerContent: this.get('initialfootertext')
        });

        // Hide and then render the dialogue.
        this.hide();
        this.render();

        // Hook into a few useful areas.
        this.bb = this.get('boundingBox');

        // Add an additional class to the boundingbox to allow tooltip-specific style to be
        // set.
        this.bb.addClass('moodle-dialogue-tooltip');

        // Change the alignment if this is an RTL language.
        if (right_to_left()) {
            this.alignpoints = [
                Y.WidgetPositionAlign.TR,
                Y.WidgetPositionAlign.LC
            ];
        }

        // Set up the text cache if it's not set up already.
        if (!this.get('textcache')) {
            this.set('textcache', new Y.Cache({
                // Set a reasonable maximum cache size to prevent memory growth.
                max: this.get('textcachesize')
            }));
        }

        // Disable the textcache when in developerdebug.
        if (M.cfg.developerdebug) {
            this.get('textcache').set('max', 0);
        }

        return this;
    },

    /**
     * Display the tooltip for the clicked link.
     *
     * The anchor for the clicked link is used.
     *
     * @method display_panel
     * @param {EventFacade} e The event from the clicked link. This is used to determine the clicked URL.
     */
    display_panel: function(e) {
        var clickedlink, thisevent, ajaxurl, config, cacheentry;

        // Prevent the default click action and prevent the event triggering anything else.
        e.preventDefault();
        e.stopPropagation();

        // Cancel any existing listeners and close the panel if it's already open.
        this.cancel_events();

        // Grab the clickedlink - this contains the URL we fetch and we align the panel to it.
        clickedlink = e.target.ancestor('a', true);

        // Reset the initial text to a spinner while we retrieve the text.
        this.setAttrs({
            headerContent: this.get('initialheadertext'),
            bodyContent: this.get('initialbodytext'),
            footerContent: this.get('initialfootertext')
        });

        // Now that initial setup has begun, show the panel.
        this.show();

        // Align with the link that was clicked.
        this.align(clickedlink, this.alignpoints);

        // Add some listen events to close on.
        thisevent = this.bb.delegate('click', this.close_panel, SELECTORS.CLOSEBUTTON, this);
        this.listenevents.push(thisevent);

        thisevent = Y.one('body').on('key', this.close_panel, 'esc', this);
        this.listenevents.push(thisevent);

        // Listen for mousedownoutside events - clickoutside is broken on IE.
        thisevent = this.bb.on('mousedownoutside', this.close_panel, this);
        this.listenevents.push(thisevent);

        // Modify the URL as required.
        ajaxurl = Y.bind(this.get('urlmodifier'), this, clickedlink.get('href'))();

        cacheentry = this.get('textcache').retrieve(ajaxurl);
        if (cacheentry) {
            // The data from this help call was already cached so use that and avoid an AJAX call.
            this._set_panel_contents(cacheentry.response);
        } else {
            // Retrieve the actual help text we should use.
            config = {
                method: 'get',
                context: this,
                sync: false,
                on: {
                    complete: function(tid, response) {
                        this._set_panel_contents(response.responseText, ajaxurl);
                    }
                }
            };

            Y.io(ajaxurl, config);
        }
    },

    _set_panel_contents: function(response, ajaxurl) {
        var responseobject;

        // Attempt to parse the response into an object.
        try {
            responseobject = Y.JSON.parse(response);
            if (responseobject.error) {
                this.close_panel();
                return new M.core.ajaxException(responseobject);
            }
        } catch (error) {
            this.close_panel();
            return new M.core.exception(error);
        }

        // Set the contents using various handlers.
        // We must use Y.bind to ensure that the correct context is used when the default handlers are overridden.
        Y.bind(this.get('headerhandler'), this, responseobject)();
        Y.bind(this.get('bodyhandler'), this, responseobject)();
        Y.bind(this.get('footerhandler'), this, responseobject)();

        if (ajaxurl) {
            // Ensure that this data is added to the cache.
            this.get('textcache').add(ajaxurl, response);
        }

        this.get('buttons').header[0].focus();
    },

    set_header_content: function(responseobject) {
        this.set('headerContent', responseobject.heading);
    },

    set_body_content: function(responseobject) {
        var bodycontent = Y.Node.create('<div />')
            .set('innerHTML', responseobject.text)
            .setAttribute('role', 'alert')
            .addClass(CSS.PANELTEXT);
        this.set('bodyContent', bodycontent);
    },

    modify_url: function(url) {
        return url.replace(/\.php\?/, '_ajax.php?');
    },

    close_panel: function(e) {
        // Hide the panel first.
        this.hide(e);

        // Cancel the listeners that we added in display_panel.
        this.cancel_events();

        // Prevent any default click that the close button may have.
        if (e) {
            e.preventDefault();
        }
    },

    cancel_events: function() {
        // Detach all listen events to prevent duplicate triggers.
        var thisevent;
        while (this.listenevents.length) {
            thisevent = this.listenevents.shift();
            thisevent.detach();
        }
    }
});
M.core = M.core || {};
M.core.tooltip = M.core.tooltip = TOOLTIP;


}, '@VERSION@', {
    "requires": [
        "base",
        "node",
        "io-base",
        "moodle-core-notification",
        "json-parse",
        "widget-position",
        "widget-position-align",
        "event-outside",
        "cache-base"
    ]
});
YUI.add('moodle-core-dock-loader', function (Y, NAME) {

var LOADERNAME = 'moodle-core-dock-loader';

/**
 * Core namespace.
 *
 * @static
 * @namespace M
 * @class core
 */
M.core = M.core || {};

/**
 * Dock namespace.
 *
 * @static
 * @namespace M.core
 * @class dock
 */
M.core.dock = M.core.dock || {};

/**
 * Creates the move to dock icon for dockable blocks if it doesn't already exist.
 *
 * @static
 * @method ensureMoveToIconExists
 * @param {Node} blocknode The Blocks node (.block[data-instanceid])
 */
M.core.dock.ensureMoveToIconExists = function(blocknode) {
    if (blocknode.one('.moveto')) {
        return true;
    }

    var commands,
        moveto = Y.Node.create('<input type="image" class="moveto customcommand requiresjs" />'),
        blockaction = blocknode.one('.block_action'),
        icon = 't/block_to_dock',
        titleh2 = blocknode.one('.header .title h2');

    // Must set the image src separately of we get an error with XML strict headers
    if (Y.one(document.body).hasClass('dir-rtl')) {
        icon = icon + '_rtl';
    }
    moveto.setAttribute('alt', M.util.get_string('addtodock', 'block'));
    if (titleh2) {
        moveto.setAttribute('title', Y.Escape.html(M.util.get_string('dockblock', 'block', titleh2.getHTML())));
    }
    moveto.setAttribute('src', M.util.image_url(icon, 'moodle'));

    if (blockaction) {
        blockaction.prepend(moveto);
    } else {
        commands = blocknode.one('.header .title .commands');
        if (!commands && blocknode.one('.header .title')) {
            commands = Y.Node.create('<div class="commands"></div>');
            blocknode.one('.header .title').append(commands);
        }
        commands.append(moveto);
    }
    return true;
};

/**
 * Dock loader.
 *
 * The dock loader is repsponsible for loading and initialising the dock only when required.
 * By doing this we avoid the need to load unnecessary JavaScript into the page for the dock just incase
 * it is being used.
 *
 * @static
 * @namespace M.core.dock
 * @class Loader
 */
M.core.dock.loader = M.core.dock.loader || {};

/**
 * Delegation events
 * @property delegationEvents
 * @protected
 * @type {Array}
 */
M.core.dock.loader.delegationEvents = [];

/**
 * Initialises the dock loader.
 *
 * The dock loader works by either firing the dock immediately if there are already docked blocks.
 * Or if there are not any docked blocks delegating two events and then loading and firing the dock when one of
 * those delegated events is triggered.
 *
 * @method initLoader
 */
M.core.dock.loader.initLoader = function() {
    Y.log('Dock loader initialising', 'debug', LOADERNAME);
    var dockedblocks = Y.all('.block[data-instanceid][data-dockable]'),
        body = Y.one(document.body),
        callback;
    dockedblocks.each(function() {
        var id = parseInt(this.getData('instanceid'), 10);
        Y.log('Dock loader watching block with instance id: '+id, 'debug', LOADERNAME);
        M.core.dock.ensureMoveToIconExists(this);
    });
    if (dockedblocks.some(function(node){return node.hasClass('dock_on_load');})) {
        Y.log('Loading dock module', 'debug', LOADERNAME);
        Y.use('moodle-core-dock', function() {
            M.core.dock.init();
        });
    } else {
        callback = function(e) {
            var i,
                block = this.ancestor('.block[data-instanceid]'),
                instanceid = block.getData('instanceid');
            e.halt();
            for (i in M.core.dock.loader.delegationEvents) {
                if (Y.Lang.isNumber(i) || Y.Lang.isString(i)) {
                    M.core.dock.loader.delegationEvents[i].detach();
                }
            }
            block.addClass('dock_on_load');
            Y.log('Loading dock module', 'debug', LOADERNAME);
            Y.use('moodle-core-dock', function(){
                M.util.set_user_preference('docked_block_instance_'+instanceid, 1);
                M.core.dock.init();
            });
        };
        M.core.dock.loader.delegationEvents.push(body.delegate('click', callback, '.moveto'));
        M.core.dock.loader.delegationEvents.push(body.delegate('key', callback, '.moveto', 'enter'));
    }
};


}, '@VERSION@', {"requires": ["escape"]});
YUI.add('moodle-core-notification-dialogue', function (Y, NAME) {

var DIALOGUE_PREFIX,
    BASE,
    CONFIRMYES,
    CONFIRMNO,
    TITLE,
    QUESTION,
    CSS;

DIALOGUE_PREFIX = 'moodle-dialogue',
BASE = 'notificationBase',
CONFIRMYES = 'yesLabel',
CONFIRMNO = 'noLabel',
TITLE = 'title',
QUESTION = 'question',
CSS = {
    BASE : 'moodle-dialogue-base',
    WRAP : 'moodle-dialogue-wrap',
    HEADER : 'moodle-dialogue-hd',
    BODY : 'moodle-dialogue-bd',
    CONTENT : 'moodle-dialogue-content',
    FOOTER : 'moodle-dialogue-ft',
    HIDDEN : 'hidden',
    LIGHTBOX : 'moodle-dialogue-lightbox'
};

// Set up the namespace once.
M.core = M.core || {};
/**
 * The generic dialogue class for use in Moodle.
 *
 * @module moodle-core-notification
 * @submodule moodle-core-notification-dialogue
 */

var DIALOGUE_NAME = 'Moodle dialogue',
    DIALOGUE,
    DIALOGUE_FULLSCREEN_CLASS = DIALOGUE_PREFIX + '-fullscreen',
    DIALOGUE_HIDDEN_CLASS = DIALOGUE_PREFIX + '-hidden',
    DIALOGUE_SELECTOR =' [role=dialog]',
    MENUBAR_SELECTOR = '[role=menubar]',
    DOT = '.',
    HAS_ZINDEX = 'moodle-has-zindex',
    CAN_RECEIVE_FOCUS_SELECTOR = 'input:not([type="hidden"]), a[href], button, textarea, select, [tabindex]';

/**
 * A re-usable dialogue box with Moodle classes applied.
 *
 * @param {Object} c Object literal specifying the dialogue configuration properties.
 * @constructor
 * @class M.core.dialogue
 * @extends Y.Panel
 */
DIALOGUE = function(c) {
    var config = Y.clone(c);
    config.COUNT = Y.stamp(this);
    var id = 'moodle-dialogue-' + config.COUNT;
    config.notificationBase =
        Y.Node.create('<div class="'+CSS.BASE+'">')
              .append(Y.Node.create('<div id="'+id+'" role="dialog" aria-labelledby="'+id+'-header-text" class="'+CSS.WRAP+'"></div>')
              .append(Y.Node.create('<div id="'+id+'-header-text" class="'+CSS.HEADER+' yui3-widget-hd"></div>'))
              .append(Y.Node.create('<div class="'+CSS.BODY+' yui3-widget-bd"></div>'))
              .append(Y.Node.create('<div class="'+CSS.FOOTER+' yui3-widget-ft"></div>')));
    Y.one(document.body).append(config.notificationBase);

    if (config.additionalBaseClass) {
        config.notificationBase.addClass(config.additionalBaseClass);
    }

    config.srcNode =    '#'+id;
    config.render =     (typeof config.render !== 'undefined') ? config.render : true;
    config.width =      config.width || '400px';
    if (typeof config.center === 'undefined') {
        config.center = true;
    } else {
        config.center = config.centered && true;
    }
    config.centered =   false;

    if (config.width === 'auto') {
        delete config.width;
    }

    // lightbox param to keep the stable versions API.
    if (config.lightbox !== false) {
        config.modal = true;
    }
    delete config.lightbox;

    // closeButton param to keep the stable versions API.
    if (config.closeButton === false) {
        config.buttons = null;
    } else {
        config.buttons = [
            {
                section: Y.WidgetStdMod.HEADER,
                classNames: 'closebutton',
                action: function () {
                    this.hide();
                }
            }
        ];
    }
    DIALOGUE.superclass.constructor.apply(this, [config]);

    if (config.closeButton !== false) {
        // The buttons constructor does not allow custom attributes
        this.get('buttons').header[0].setAttribute('title', this.get('closeButtonTitle'));
    }
};
Y.extend(DIALOGUE, Y.Panel, {
    // Window resize event listener.
    _resizeevent : null,
    // Orientation change event listener.
    _orientationevent : null,
    _calculatedzindex : false,
    /**
     * Initialise the dialogue.
     *
     * @method initializer
     * @return void
     */
    initializer : function() {
        var bb;

        if (this.get('render') && !this.get('rendered')) {
            this.render();
        }

        this.makeResponsive();
        this.after('visibleChange', this.visibilityChanged, this);
        if (this.get('center')) {
            this.centerDialogue();
        }

        if (this.get('modal')) {
            this.plug(Y.M.core.LockScroll);
        }

        // Workaround upstream YUI bug http://yuilibrary.com/projects/yui3/ticket/2532507
        // and allow setting of z-index in theme.
        bb = this.get('boundingBox');
        bb.addClass(HAS_ZINDEX);

        // Add any additional classes that were specified.
        Y.Array.each(this.get('extraClasses'), bb.addClass, bb);

        if (this.get('visible')) {
            this.applyZIndex();
        }
        // Recalculate the zIndex every time the modal is altered.
        this.on('maskShow', this.applyZIndex);
        // We must show - after the dialogue has been positioned,
        // either by centerDialogue or makeResonsive. This is because the show() will trigger
        // a focus on the dialogue, which will scroll the page. If the dialogue has not
        // been positioned it will scroll back to the top of the page.
        if (this.get('visible')) {
            this.show();
            this.keyDelegation();
        }
    },

    /**
     * Either set the zindex to the supplied value, or set it to one more than the highest existing
     * dialog in the page.
     *
     * @method visibilityChanged
     * @return void
     */
    applyZIndex : function() {
        var highestzindex = 1,
            zindexvalue = 1,
            bb = this.get('boundingBox'),
            ol = this.get('maskNode'),
            zindex = this.get('zIndex');
        if (zindex !== 0 && !this._calculatedzindex) {
            // The zindex was specified so we should use that.
            bb.setStyle('zIndex', zindex);
        } else {
            // Determine the correct zindex by looking at all existing dialogs and menubars in the page.
            Y.all(DIALOGUE_SELECTOR + ', ' + MENUBAR_SELECTOR + ', ' + DOT + HAS_ZINDEX).each(function (node) {
                var zindex = this.findZIndex(node);
                if (zindex > highestzindex) {
                    highestzindex = zindex;
                }
            }, this);
            // Only set the zindex if we found a wrapper.
            zindexvalue = (highestzindex + 1).toString();
            bb.setStyle('zIndex', zindexvalue);
            this.set('zIndex', zindexvalue);
            if (this.get('modal')) {
                ol.setStyle('zIndex', zindexvalue);
            }
            this._calculatedzindex = true;
        }
    },

    /**
     * Finds the zIndex of the given node or its parent.
     *
     * @method findZIndex
     * @param Node node
     * @returns int Return either the zIndex of 0 if one was not found.
     */
    findZIndex : function(node) {
        // In most cases the zindex is set on the parent of the dialog.
        var zindex = node.getStyle('zIndex') || node.ancestor().getStyle('zIndex');
        if (zindex) {
            return parseInt(zindex, 10);
        }
        return 0;
    },

    /**
     * Event listener for the visibility changed event.
     *
     * @method visibilityChanged
     * @return void
     */
    visibilityChanged : function(e) {
        var titlebar, bb;
        if (e.attrName === 'visible') {
            this.get('maskNode').addClass(CSS.LIGHTBOX);
            if (e.prevVal && !e.newVal) {
                bb = this.get('boundingBox');
                if (this._resizeevent) {
                    this._resizeevent.detach();
                    this._resizeevent = null;
                }
                if (this._orientationevent) {
                    this._orientationevent.detach();
                    this._orientationevent = null;
                }
                bb.detach('key', this.keyDelegation);
            }
            if (!e.prevVal && e.newVal) {
                // This needs to be done each time the dialog is shown as new dialogs may have been opened.
                this.applyZIndex();
                // This needs to be done each time the dialog is shown as the window may have been resized.
                this.makeResponsive();
                if (!this.shouldResizeFullscreen()) {
                    if (this.get('draggable')) {
                        titlebar = '#' + this.get('id') + ' .' + CSS.HEADER;
                        this.plug(Y.Plugin.Drag, {handles : [titlebar]});
                        Y.one(titlebar).setStyle('cursor', 'move');
                    }
                }
                this.keyDelegation();
            }
            if (this.get('center') && !e.prevVal && e.newVal) {
                this.centerDialogue();
            }
        }
    },
    /**
     * If the responsive attribute is set on the dialog, and the window size is
     * smaller than the responsive width - make the dialog fullscreen.
     *
     * @method makeResponsive
     * @return void
     */
    makeResponsive : function() {
        var bb = this.get('boundingBox'),
            content;

        if (this.shouldResizeFullscreen()) {
            // Make this dialogue fullscreen on a small screen.
            // Disable the page scrollbars.

            // Size and position the fullscreen dialog.

            bb.addClass(DIALOGUE_FULLSCREEN_CLASS);
            bb.setStyles({'left' : null,
                          'top' : null,
                          'width' : null,
                          'height' : null,
                          'right' : null,
                          'bottom' : null});

            content = Y.one('#' + this.get('id') + ' .' + CSS.BODY);
        } else {
            if (this.get('responsive')) {
                // We must reset any of the fullscreen changes.
                bb.removeClass(DIALOGUE_FULLSCREEN_CLASS)
                    .setStyles({'width' : this.get('width'),
                                'height' : this.get('height')});
                content = Y.one('#' + this.get('id') + ' .' + CSS.BODY);
            }
        }
    },
    /**
     * Center the dialog on the screen.
     *
     * @method centerDialogue
     * @return void
     */
    centerDialogue : function() {
        var bb = this.get('boundingBox'),
            hidden = bb.hasClass(DIALOGUE_HIDDEN_CLASS),
            x,
            y;

        // Don't adjust the position if we are in full screen mode.
        if (this.shouldResizeFullscreen()) {
            return;
        }
        if (hidden) {
            bb.setStyle('top', '-1000px').removeClass(DIALOGUE_HIDDEN_CLASS);
        }
        x = Math.max(Math.round((bb.get('winWidth') - bb.get('offsetWidth'))/2), 15);
        y = Math.max(Math.round((bb.get('winHeight') - bb.get('offsetHeight'))/2), 15) + Y.one(window).get('scrollTop');
        bb.setStyles({ 'left' : x, 'top' : y});

        if (hidden) {
            bb.addClass(DIALOGUE_HIDDEN_CLASS);
        }
    },
    /**
     * Return if this dialogue should be fullscreen or not.
     * Responsive attribute must be true and we should not be in an iframe and the screen width should
     * be less than the responsive width.
     *
     * @method shouldResizeFullscreen
     * @return Boolean
     */
    shouldResizeFullscreen : function() {
        return (window === window.parent) && this.get('responsive') &&
               Math.floor(Y.one(document.body).get('winWidth')) < this.get('responsiveWidth');
    },

    /**
     * Override the show method to set keyboard focus on the dialogue.
     *
     * @method show
     * @return void
     */
    show : function() {
        var result = null,
            header = this.headerNode,
            content = this.bodyNode;

        result = DIALOGUE.superclass.show.call(this);

        // Lock scroll if the plugin is present.
        if (this.lockScroll) {
            // We need to force the scroll locking for full screen dialogues, even if they have a small vertical size to
            // prevent the background scrolling while the dialogue is open.
            this.lockScroll.enableScrollLock(this.shouldResizeFullscreen());
        }

        if (header && header !== '') {
            header.focus();
        } else if (content && content !== '') {
            content.focus();
        }
        return result;
    },

    hide: function(e) {
        if (e) {
            // If the event was closed by an escape key event, then we need to check that this
            // dialogue is currently focused to prevent closing all dialogues in the stack.
            if (e.type === 'key' && e.keyCode === 27 && !this.get('focused')) {
                return;
            }
        }

        // Unlock scroll if the plugin is present.
        if (this.lockScroll) {
            this.lockScroll.disableScrollLock();
        }

        return DIALOGUE.superclass.hide.call(this, arguments);
    },
    /**
     * Setup key delegation to keep tabbing within the open dialogue.
     *
     * @method keyDelegation
     */
    keyDelegation : function() {
        var bb = this.get('boundingBox');
        bb.delegate('key', function(e){
            var target = e.target;
            var direction = 'forward';
            if (e.shiftKey) {
                direction = 'backward';
            }
            if (this.trapFocus(target, direction)) {
                e.preventDefault();
            }
        }, 'down:9', CAN_RECEIVE_FOCUS_SELECTOR, this);
    },
    /**
     * Trap the tab focus within the open modal.
     *
     * @param string target the element target
     * @param string direction tab key for forward and tab+shift for backward
     * @returns bool
     */
    trapFocus : function(target, direction) {
        var bb = this.get('boundingBox'),
            firstitem = bb.one(CAN_RECEIVE_FOCUS_SELECTOR),
            lastitem = bb.all(CAN_RECEIVE_FOCUS_SELECTOR).pop();

        if (target === lastitem && direction === 'forward') { // Tab key.
            return firstitem.focus();
        } else if (target === firstitem && direction === 'backward') {  // Tab+shift key.
            return lastitem.focus();
        }
    }
}, {
    NAME : DIALOGUE_NAME,
    CSS_PREFIX : DIALOGUE_PREFIX,
    ATTRS : {
        notificationBase : {

        },

        /**
         * Whether to display the dialogue modally and with a
         * lightbox style.
         *
         * @attribute lightbox
         * @type Boolean
         * @default true
         */
        lightbox : {
            validator : Y.Lang.isBoolean,
            value : true
        },

        /**
         * Whether to display a close button on the dialogue.
         *
         * Note, we do not recommend hiding the close button as this has
         * potential accessibility concerns.
         *
         * @attribute closeButton
         * @type Boolean
         * @default true
         */
        closeButton : {
            validator : Y.Lang.isBoolean,
            value : true
        },

        /**
         * The title for the close button if one is to be shown.
         *
         * @attribute closeButtonTitle
         * @type String
         * @default 'Close'
         */
        closeButtonTitle : {
            validator : Y.Lang.isString,
            value: M.util.get_string('closebuttontitle', 'moodle')
        },

        /**
         * Whether to display the dialogue centrally on the screen.
         *
         * @attribute center
         * @type Boolean
         * @default true
         */
        center : {
            validator : Y.Lang.isBoolean,
            value : true
        },

        /**
         * Whether to make the dialogue movable around the page.
         *
         * @attribute draggable
         * @type Boolean
         * @default false
         */
        draggable : {
            validator : Y.Lang.isBoolean,
            value : false
        },

        /**
         * Used to generate a unique id for the dialogue.
         *
         * @attribute COUNT
         * @type String
         * @default null
         */
        COUNT: {
            value: null
        },

        /**
         * Used to disable the fullscreen resizing behaviour if required.
         *
         * @attribute responsive
         * @type Boolean
         * @default true
         */
        responsive : {
            validator : Y.Lang.isBoolean,
            value : true
        },

        /**
         * The width that this dialogue should be resized to fullscreen.
         *
         * @attribute responsiveWidth
         * @type Integer
         * @default 768
         */
        responsiveWidth : {
            value : 768
        },

        /**
         * Any additional classes to add to the boundingBox.
         *
         * @attributes extraClasses
         * @type Array
         * @default []
         */
        extraClasses: {
            value: []
        }
    }
});

M.core.dialogue = DIALOGUE;


}, '@VERSION@', {"requires": ["base", "node", "panel", "escape", "event-key", "dd-plugin", "moodle-core-lockscroll"]});
