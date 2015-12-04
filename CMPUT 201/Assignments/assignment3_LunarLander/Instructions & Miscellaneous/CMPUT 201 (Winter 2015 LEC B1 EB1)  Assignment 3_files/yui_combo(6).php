YUI.add('moodle-core-lockscroll', function (Y, NAME) {

/**
 * Provides the ability to lock the scroll for a page.
 *
 * @module moodle-core-lockscroll
 */

/**
 * Provides the ability to lock the scroll for a page.
 *
 * This is achieved by applying the class 'lockscroll' to the body Node.
 *
 * Nested widgets are also supported and the scroll lock is only removed
 * when the final plugin instance is disabled.
 *
 * @class M.core.LockScroll
 * @extends Plugin.Base
 */
Y.namespace('M.core').LockScroll = Y.Base.create('lockScroll', Y.Plugin.Base, [], {

    /**
     * Whether the LockScroll has been activated.
     *
     * @property _enabled
     * @type Boolean
     * @protected
     */
    _enabled: false,

    /**
     * Handle destruction of the lockScroll instance, including disabling
     * of the current instance.
     *
     * @method destructor
     */
    destructor: function() {
        this.disableScrollLock();
    },

    /**
     * Start locking the page scroll.
     *
     * This is achieved by applying the lockscroll class to the body Node.
     *
     * A count of the total number of active, and enabled, lockscroll instances is also kept on
     * the body to ensure that premature disabling does not occur.
     *
     * @method enableScrollLock
     * @param {Boolean} forceOnSmallWindow Whether to enable the scroll lock, even for small window sizes.
     * @chainable
     */
    enableScrollLock: function(forceOnSmallWindow) {
        if (this.isActive()) {
            Y.log('LockScroll already active. Ignoring enable request', 'warn', 'moodle-core-lockscroll');
            return;
        }

        var dialogueHeight = this.get('host').get('boundingBox').get('region').height,
            // Most modern browsers use win.innerHeight, but some older versions of IE use documentElement.clientHeight.
            // We fall back to 0 if neither can be found which has the effect of disabling scroll locking.
            windowHeight = Y.config.win.innerHeight || Y.config.doc.documentElement.clientHeight || 0;

        if (!forceOnSmallWindow && dialogueHeight > (windowHeight - 10)) {
            Y.log('Dialogue height greater than window height. Ignoring enable request.', 'warn', 'moodle-core-lockscroll');
            return;
        }

        Y.log('Enabling LockScroll.', 'debug', 'moodle-core-lockscroll');
        this._enabled = true;
        var body = Y.one(Y.config.doc.body);

        // We use a CSS class on the body to handle the actual locking.
        body.addClass('lockscroll');

        // Increase the count of active instances - this is used to ensure that we do not
        // remove the locking when parent windows are still open.
        // Note: We cannot use getData here because data attributes are sandboxed to the instance that created them.
        var currentCount = parseInt(body.getAttribute('data-activeScrollLocks'), 10) || 0,
            newCount = currentCount + 1;
        body.setAttribute('data-activeScrollLocks', newCount);
        Y.log("Setting the activeScrollLocks count from " + currentCount + " to " + newCount,
                'debug', 'moodle-core-lockscroll');

        return this;
    },

    /**
     * Stop locking the page scroll.
     *
     * The instance may be disabled but the scroll lock not removed if other instances of the
     * plugin are also active.
     *
     * @method disableScrollLock
     * @chainable
     */
    disableScrollLock: function() {
        if (this.isActive()) {
            Y.log('Disabling LockScroll.', 'debug', 'moodle-core-lockscroll');
            this._enabled = false;

            var body = Y.one(Y.config.doc.body);

            // Decrease the count of active instances.
            // Note: We cannot use getData here because data attributes are sandboxed to the instance that created them.
            var currentCount = parseInt(body.getAttribute('data-activeScrollLocks'), 10) || 1,
                newCount = currentCount - 1;

            if (currentCount === 1) {
                body.removeClass('lockscroll');
            }

            body.setAttribute('data-activeScrollLocks', currentCount - 1);
            Y.log("Setting the activeScrollLocks count from " + currentCount + " to " + newCount,
                    'debug', 'moodle-core-lockscroll');
        }

        return this;
    },

    /**
     * Return whether scroll locking is active.
     *
     * @method isActive
     * @return Boolean
     */
    isActive: function() {
        return this._enabled;
    }

}, {
    NS: 'lockScroll',
    ATTRS: {
    }
});


}, '@VERSION@', {"requires": ["plugin", "base-build"]});
YUI.add('moodle-core-notification-alert', function (Y, NAME) {

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
 * A dialogue type designed to display an alert to the user.
 *
 * @module moodle-core-notification
 * @submodule moodle-core-notification-alert
 */

var ALERT_NAME = 'Moodle alert',
    ALERT;

/**
 * Extends core Dialogue to show the alert dialogue.
 *
 * @param {Object} config Object literal specifying the dialogue configuration properties.
 * @constructor
 * @class M.core.alert
 * @extends M.core.dialogue
 */
ALERT = function(config) {
    config.closeButton = false;
    ALERT.superclass.constructor.apply(this, [config]);
};
Y.extend(ALERT, M.core.dialogue, {
    /**
     * The list of events to detach when destroying this dialogue.
     *
     * @property _closeEvents
     * @type EventHandle[]
     * @private
     */
    _closeEvents: null,
    initializer : function() {
        this._closeEvents = [];
        this.publish('complete');
        var yes = Y.Node.create('<input type="button" id="id_yuialertconfirm-' + this.get('COUNT') + '" value="'+this.get(CONFIRMYES)+'" />'),
            content = Y.Node.create('<div class="confirmation-dialogue"></div>')
                    .append(Y.Node.create('<div class="confirmation-message">'+this.get('message')+'</div>'))
                    .append(Y.Node.create('<div class="confirmation-buttons"></div>')
                            .append(yes));
        this.get(BASE).addClass('moodle-dialogue-confirm');
        this.setStdModContent(Y.WidgetStdMod.BODY, content, Y.WidgetStdMod.REPLACE);
        this.setStdModContent(Y.WidgetStdMod.HEADER,
                '<h1 id="moodle-dialogue-'+this.get('COUNT')+'-header-text">' + this.get(TITLE) + '</h1>', Y.WidgetStdMod.REPLACE);
        this.after('destroyedChange', function(){this.get(BASE).remove();}, this);
        this._closeEvents.push(
            Y.on('key', this.submit, window, 'down:13', this),
            yes.on('click', this.submit, this)
        );

        var closeButton = this.get('boundingBox').one('.closebutton');
        if (closeButton) {
            // The close button should act exactly like the 'No' button.
            this._closeEvents.push(
                closeButton.on('click', this.submit, this)
            );
        }
    },
    submit : function() {
        new Y.EventHandle(this._closeEvents).detach();
        this.fire('complete');
        this.hide();
        this.destroy();
    }
}, {
    NAME : ALERT_NAME,
    CSS_PREFIX : DIALOGUE_PREFIX,
    ATTRS : {

        /**
         * The title of the alert.
         *
         * @attribute title
         * @type String
         * @default 'Alert'
         */
        title : {
            validator : Y.Lang.isString,
            value : 'Alert'
        },

        /**
         * The message of the alert.
         *
         * @attribute message
         * @type String
         * @default 'Confirm'
         */
        message : {
            validator : Y.Lang.isString,
            value : 'Confirm'
        },

        /**
         * The button text to use to accept the alert.
         *
         * @attribute yesLabel
         * @type String
         * @default 'Ok'
         */
        yesLabel : {
            validator : Y.Lang.isString,
            setter : function(txt) {
                if (!txt) {
                    txt = 'Ok';
                }
                return txt;
            },
            value : 'Ok'
        }
    }
});

M.core.alert = ALERT;


}, '@VERSION@', {"requires": ["moodle-core-notification-dialogue"]});
YUI.add('moodle-core-notification-confirm', function (Y, NAME) {

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
 * A dialogue type designed to display a confirmation to the user.
 *
 * @module moodle-core-notification
 * @submodule moodle-core-notification-confirm
 */

var CONFIRM_NAME = 'Moodle confirmation dialogue',
    CONFIRM;

/**
 * Extends core Dialogue to show the confirmation dialogue.
 *
 * @param {Object} config Object literal specifying the dialogue configuration properties.
 * @constructor
 * @class M.core.confirm
 * @extends M.core.dialogue
 */
CONFIRM = function(config) {
    CONFIRM.superclass.constructor.apply(this, [config]);
};
Y.extend(CONFIRM, M.core.dialogue, {
    /**
     * The list of events to detach when destroying this dialogue.
     *
     * @property _closeEvents
     * @type EventHandle[]
     * @private
     */
    _closeEvents: null,
    initializer: function() {
        this._closeEvents = [];
        this.publish('complete');
        this.publish('complete-yes');
        this.publish('complete-no');
        var yes = Y.Node.create('<input type="button" id="id_yuiconfirmyes-' + this.get('COUNT') + '" value="'+this.get(CONFIRMYES)+'" />'),
            no = Y.Node.create('<input type="button" id="id_yuiconfirmno-' + this.get('COUNT') + '" value="'+this.get(CONFIRMNO)+'" />'),
            content = Y.Node.create('<div class="confirmation-dialogue"></div>')
                        .append(Y.Node.create('<div class="confirmation-message">'+this.get(QUESTION)+'</div>'))
                        .append(Y.Node.create('<div class="confirmation-buttons"></div>')
                            .append(yes)
                            .append(no));
        this.get(BASE).addClass('moodle-dialogue-confirm');
        this.setStdModContent(Y.WidgetStdMod.BODY, content, Y.WidgetStdMod.REPLACE);
        this.setStdModContent(Y.WidgetStdMod.HEADER,
                '<h1 id="moodle-dialogue-'+this.get('COUNT')+'-header-text">' + this.get(TITLE) + '</h1>', Y.WidgetStdMod.REPLACE);
        this.after('destroyedChange', function(){this.get(BASE).remove();}, this);

        this._closeEvents.push(
            Y.on('key', this.submit, window, 'down:27', this, false),
            yes.on('click', this.submit, this, true),
            no.on('click', this.submit, this, false)
        );

        var closeButton = this.get('boundingBox').one('.closebutton');
        if (closeButton) {
            // The close button should act exactly like the 'No' button.
            this._closeEvents.push(
                closeButton.on('click', this.submit, this)
            );
        }
    },
    submit: function(e, outcome) {
        new Y.EventHandle(this._closeEvents).detach();
        this.fire('complete', outcome);
        if (outcome) {
            this.fire('complete-yes');
        } else {
            this.fire('complete-no');
        }
        this.hide();
        this.destroy();
    }
}, {
    NAME: CONFIRM_NAME,
    CSS_PREFIX: DIALOGUE_PREFIX,
    ATTRS: {

        /**
         * The button text to use to accept the confirmation.
         *
         * @attribute yesLabel
         * @type String
         * @default 'Yes'
         */
        yesLabel: {
            validator: Y.Lang.isString,
            value: M.util.get_string('yes', 'moodle')
        },

        /**
         * The button text to use to reject the confirmation.
         *
         * @attribute noLabel
         * @type String
         * @default 'No'
         */
        noLabel: {
            validator: Y.Lang.isString,
            value: M.util.get_string('no', 'moodle')
        },

        /**
         * The title of the dialogue.
         *
         * @attribute title
         * @type String
         * @default 'Confirm'
         */
        title: {
            validator: Y.Lang.isString,
            value: M.util.get_string('confirm', 'moodle')
        },

        /**
         * The question posed by the dialogue.
         *
         * @attribute question
         * @type String
         * @default 'Are you sure?'
         */
        question: {
            validator: Y.Lang.isString,
            value: M.util.get_string('areyousure', 'moodle')
        }
    }
});
Y.augment(CONFIRM, Y.EventTarget);

M.core.confirm = CONFIRM;


}, '@VERSION@', {"requires": ["moodle-core-notification-dialogue"]});
YUI.add('moodle-core-notification-exception', function (Y, NAME) {

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
 * A dialogue type designed to display an appropriate error when a generic
 * javascript error was thrown and caught.
 *
 * @module moodle-core-notification
 * @submodule moodle-core-notification-exception
 */

var EXCEPTION_NAME = 'Moodle exception',
    EXCEPTION;

/**
 * Extends core Dialogue to show the exception dialogue.
 *
 * @param {Object} config Object literal specifying the dialogue configuration properties.
 * @constructor
 * @class M.core.exception
 * @extends M.core.dialogue
 */
EXCEPTION = function(c) {
    var config = Y.mix({}, c);
    config.width = config.width || (M.cfg.developerdebug)?Math.floor(Y.one(document.body).get('winWidth')/3)+'px':null;
    config.closeButton = true;

    // We need to whitelist some properties which are part of the exception
    // prototype, otherwise AttributeCore filters them during value normalisation.
    var whitelist = [
        'message',
        'name',
        'fileName',
        'lineNumber',
        'stack'
    ];
    Y.Array.each(whitelist, function(k) {
        config[k] = c[k];
    });

    EXCEPTION.superclass.constructor.apply(this, [config]);
};
Y.extend(EXCEPTION, M.core.dialogue, {
    _hideTimeout : null,
    _keypress : null,
    initializer : function(config) {
        var content,
            self = this,
            delay = this.get('hideTimeoutDelay');
        this.get(BASE).addClass('moodle-dialogue-exception');
        this.setStdModContent(Y.WidgetStdMod.HEADER,
                '<h1 id="moodle-dialogue-'+config.COUNT+'-header-text">' + Y.Escape.html(config.name) + '</h1>',
                Y.WidgetStdMod.REPLACE);
        content = Y.Node.create('<div class="moodle-exception"></div>')
                .append(Y.Node.create('<div class="moodle-exception-message">'+Y.Escape.html(this.get('message'))+'</div>'))
                .append(Y.Node.create('<div class="moodle-exception-param hidden param-filename"><label>File:</label> ' +
                        Y.Escape.html(this.get('fileName'))+'</div>'))
                .append(Y.Node.create('<div class="moodle-exception-param hidden param-linenumber"><label>Line:</label> ' +
                        Y.Escape.html(this.get('lineNumber'))+'</div>'))
                .append(Y.Node.create('<div class="moodle-exception-param hidden param-stacktrace"><label>Stack trace:</label> <pre>' +
                        this.get('stack')+'</pre></div>'));
        if (M.cfg.developerdebug) {
            content.all('.moodle-exception-param').removeClass('hidden');
        }
        this.setStdModContent(Y.WidgetStdMod.BODY, content, Y.WidgetStdMod.REPLACE);

        if (delay) {
            this._hideTimeout = setTimeout(function(){self.hide();}, delay);
        }
        this.after('visibleChange', this.visibilityChanged, this);
        this.after('destroyedChange', function(){this.get(BASE).remove();}, this);
        this._keypress = Y.on('key', this.hide, window, 'down:13,27', this);
        this.centerDialogue();
    },
    visibilityChanged : function(e) {
        if (e.attrName === 'visible' && e.prevVal && !e.newVal) {
            if (this._keypress) {
                this._keypress.detach();
            }
            var self = this;
            setTimeout(function(){self.destroy();}, 1000);
        }
    }
}, {
    NAME : EXCEPTION_NAME,
    CSS_PREFIX : DIALOGUE_PREFIX,
    ATTRS : {
        /**
         * The message of the alert.
         *
         * @attribute message
         * @type String
         * @default ''
         */
        message : {
            value : ''
        },

        /**
         * The name of the alert.
         *
         * @attribute title
         * @type String
         * @default ''
         */
        name : {
            value : ''
        },

        /**
         * The name of the file where the error was thrown.
         *
         * @attribute fileName
         * @type String
         * @default ''
         */
        fileName : {
            value : ''
        },

        /**
         * The line number where the error was thrown.
         *
         * @attribute lineNumber
         * @type String
         * @default ''
         */
        lineNumber : {
            value : ''
        },

        /**
         * The backtrace from the error
         *
         * @attribute lineNumber
         * @type String
         * @default ''
         */
        stack : {
            setter : function(str) {
                var lines = Y.Escape.html(str).split("\n"),
                    pattern = new RegExp('^(.+)@('+M.cfg.wwwroot+')?(.{0,75}).*:(\\d+)$'),
                    i;
                for (i in lines) {
                    lines[i] = lines[i].replace(pattern,
                            "<div class='stacktrace-line'>ln: $4</div><div class='stacktrace-file'>$3</div><div class='stacktrace-call'>$1</div>");
                }
                return lines.join('');
            },
            value : ''
        },

        /**
         * If set, the dialogue is hidden after the specified timeout period.
         *
         * @attribute hideTimeoutDelay
         * @type Number
         * @default null
         * @optional
         */
        hideTimeoutDelay : {
            validator : Y.Lang.isNumber,
            value : null
        }
    }
});

M.core.exception = EXCEPTION;


}, '@VERSION@', {"requires": ["moodle-core-notification-dialogue"]});
YUI.add('moodle-core-notification-ajaxexception', function (Y, NAME) {

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
 * A dialogue type designed to display an appropriate error when an error
 * thrown in the Moodle codebase was reported during an AJAX request.
 *
 * @module moodle-core-notification
 * @submodule moodle-core-notification-ajaxexception
 */

var AJAXEXCEPTION_NAME = 'Moodle AJAX exception',
    AJAXEXCEPTION;

/**
 * Extends core Dialogue to show the exception dialogue.
 *
 * @param {Object} config Object literal specifying the dialogue configuration properties.
 * @constructor
 * @class M.core.ajaxException
 * @extends M.core.dialogue
 */
AJAXEXCEPTION = function(config) {
    config.name = config.name || 'Error';
    config.closeButton = true;
    AJAXEXCEPTION.superclass.constructor.apply(this, [config]);
};
Y.extend(AJAXEXCEPTION, M.core.dialogue, {
    _keypress : null,
    initializer : function(config) {
        var content,
            self = this,
            delay = this.get('hideTimeoutDelay');
        this.get(BASE).addClass('moodle-dialogue-exception');
        this.setStdModContent(Y.WidgetStdMod.HEADER,
                '<h1 id="moodle-dialogue-'+this.get('COUNT')+'-header-text">' + Y.Escape.html(config.name) + '</h1>',
                Y.WidgetStdMod.REPLACE);
        content = Y.Node.create('<div class="moodle-ajaxexception"></div>')
                .append(Y.Node.create('<div class="moodle-exception-message">'+Y.Escape.html(this.get('error'))+'</div>'))
                .append(Y.Node.create('<div class="moodle-exception-param hidden param-debuginfo"><label>URL:</label> ' +
                        this.get('reproductionlink')+'</div>'))
                .append(Y.Node.create('<div class="moodle-exception-param hidden param-debuginfo"><label>Debug info:</label> ' +
                        Y.Escape.html(this.get('debuginfo'))+'</div>'))
                .append(Y.Node.create('<div class="moodle-exception-param hidden param-stacktrace"><label>Stack trace:</label> <pre>' +
                        Y.Escape.html(this.get('stacktrace'))+'</pre></div>'));
        if (M.cfg.developerdebug) {
            content.all('.moodle-exception-param').removeClass('hidden');
        }
        this.setStdModContent(Y.WidgetStdMod.BODY, content, Y.WidgetStdMod.REPLACE);

        if (delay) {
            this._hideTimeout = setTimeout(function(){self.hide();}, delay);
        }
        this.after('visibleChange', this.visibilityChanged, this);
        this._keypress = Y.on('key', this.hide, window, 'down:13, 27', this);
        this.centerDialogue();
    },
    visibilityChanged : function(e) {
        if (e.attrName === 'visible' && e.prevVal && !e.newVal) {
            var self = this;
            this._keypress.detach();
            setTimeout(function(){self.destroy();}, 1000);
        }
    }
}, {
    NAME : AJAXEXCEPTION_NAME,
    CSS_PREFIX : DIALOGUE_PREFIX,
    ATTRS : {

        /**
         * The error message given in the exception.
         *
         * @attribute error
         * @type String
         * @default 'Unknown error'
         * @optional
         */
        error : {
            validator : Y.Lang.isString,
            value: M.util.get_string('unknownerror', 'moodle')
        },

        /**
         * Any additional debug information given in the exception.
         *
         * @attribute stacktrace
         * @type String|null
         * @default null
         * @optional
         */
        debuginfo : {
            value : null
        },

        /**
         * The complete stack trace provided in the exception.
         *
         * @attribute stacktrace
         * @type String|null
         * @default null
         * @optional
         */
        stacktrace : {
            value : null
        },

        /**
         * A link which may be used by support staff to replicate the issue.
         *
         * @attribute reproductionlink
         * @type String
         * @default null
         * @optional
         */
        reproductionlink : {
            setter : function(link) {
                if (link !== null) {
                    link = Y.Escape.html(link);
                    link = '<a href="'+link+'">'+link.replace(M.cfg.wwwroot, '')+'</a>';
                }
                return link;
            },
            value : null
        },

        /**
         * If set, the dialogue is hidden after the specified timeout period.
         *
         * @attribute hideTimeoutDelay
         * @type Number
         * @default null
         * @optional
         */
        hideTimeoutDelay : {
            validator : Y.Lang.isNumber,
            value : null
        }
    }
});

M.core.ajaxException = AJAXEXCEPTION;


}, '@VERSION@', {"requires": ["moodle-core-notification-dialogue"]});
YUI.add('moodle-core-notification', function (Y, NAME) {

/**
 * The notification module provides a standard set of dialogues for use
 * within Moodle.
 *
 * @module moodle-core-notification
 * @main
 */

/**
 * To avoid bringing moodle-core-notification into modules in it's
 * entirety, we now recommend using on of the subclasses of
 * moodle-core-notification. These include:
 * <dl>
 *  <dt> moodle-core-notification-dialogue</dt>
 *  <dt> moodle-core-notification-alert</dt>
 *  <dt> moodle-core-notification-confirm</dt>
 *  <dt> moodle-core-notification-exception</dt>
 *  <dt> moodle-core-notification-ajaxexception</dt>
 * </dl>
 *
 * @class M.core.notification
 * @deprecated
 */
Y.log("The moodle-core-notification parent module has been deprecated. " +
        "Please use one of its subclasses instead.", 'moodle-core-notification', 'warn');


}, '@VERSION@', {
    "requires": [
        "moodle-core-notification-dialogue",
        "moodle-core-notification-alert",
        "moodle-core-notification-confirm",
        "moodle-core-notification-exception",
        "moodle-core-notification-ajaxexception"
    ]
});
/*
YUI 3.13.0 (build 508226d)
Copyright 2013 Yahoo! Inc. All rights reserved.
Licensed under the BSD License.
http://yuilibrary.com/license/
*/

YUI.add('cache-base', function (Y, NAME) {

/**
 * The Cache utility provides a common configurable interface for components to
 * cache and retrieve data from a local JavaScript struct.
 *
 * @module cache
 * @main
 */

/**
 * Provides the base class for the YUI Cache utility.
 *
 * @submodule cache-base
 */
var LANG = Y.Lang,
    isDate = Y.Lang.isDate,

/**
 * Base class for the YUI Cache utility.
 * @class Cache
 * @extends Base
 * @constructor
 */
Cache = function() {
    Cache.superclass.constructor.apply(this, arguments);
};

    /////////////////////////////////////////////////////////////////////////////
    //
    // Cache static properties
    //
    /////////////////////////////////////////////////////////////////////////////
Y.mix(Cache, {
    /**
     * Class name.
     *
     * @property NAME
     * @type String
     * @static
     * @final
     * @value "cache"
     */
    NAME: "cache",


    ATTRS: {
        /////////////////////////////////////////////////////////////////////////////
        //
        // Cache Attributes
        //
        /////////////////////////////////////////////////////////////////////////////

        /**
        * @attribute max
        * @description Maximum number of entries the Cache can hold.
        * Set to 0 to turn off caching.
        * @type Number
        * @default 0
        */
        max: {
            value: 0,
            setter: "_setMax"
        },

        /**
        * @attribute size
        * @description Number of entries currently cached.
        * @type Number
        */
        size: {
            readOnly: true,
            getter: "_getSize"
        },

        /**
        * @attribute uniqueKeys
        * @description Validate uniqueness of stored keys. Default is false and
        * is more performant.
        * @type Boolean
        */
        uniqueKeys: {
            value: false
        },

        /**
        * @attribute expires
        * @description Absolute Date when data expires or
        * relative number of milliseconds. Zero disables expiration.
        * @type Date | Number
        * @default 0
        */
        expires: {
            value: 0,
            validator: function(v) {
                return Y.Lang.isDate(v) || (Y.Lang.isNumber(v) && v >= 0);
            }
        },

        /**
         * @attribute entries
         * @description Cached entries.
         * @type Array
         */
        entries: {
            readOnly: true,
            getter: "_getEntries"
        }
    }
});

Y.extend(Cache, Y.Base, {
    /////////////////////////////////////////////////////////////////////////////
    //
    // Cache private properties
    //
    /////////////////////////////////////////////////////////////////////////////

    /**
     * Array of request/response objects indexed chronologically.
     *
     * @property _entries
     * @type Object[]
     * @private
     */
    _entries: null,

    /////////////////////////////////////////////////////////////////////////////
    //
    // Cache private methods
    //
    /////////////////////////////////////////////////////////////////////////////

    /**
    * @method initializer
    * @description Internal init() handler.
    * @param config {Object} Config object.
    * @private
    */
    initializer: function(config) {

        /**
        * @event add
        * @description Fired when an entry is added.
        * @param e {Event.Facade} Event Facade with the following properties:
         * <dl>
         * <dt>entry (Object)</dt> <dd>The cached entry.</dd>
         * </dl>
        * @preventable _defAddFn
        */
        this.publish("add", {defaultFn: this._defAddFn});

        /**
        * @event flush
        * @description Fired when the cache is flushed.
        * @param e {Event.Facade} Event Facade object.
        * @preventable _defFlushFn
        */
        this.publish("flush", {defaultFn: this._defFlushFn});

        /**
        * @event request
        * @description Fired when an entry is requested from the cache.
        * @param e {Event.Facade} Event Facade with the following properties:
        * <dl>
        * <dt>request (Object)</dt> <dd>The request object.</dd>
        * </dl>
        */

        /**
        * @event retrieve
        * @description Fired when an entry is retrieved from the cache.
        * @param e {Event.Facade} Event Facade with the following properties:
        * <dl>
        * <dt>entry (Object)</dt> <dd>The retrieved entry.</dd>
        * </dl>
        */

        // Initialize internal values
        this._entries = [];
    },

    /**
    * @method destructor
    * @description Internal destroy() handler.
    * @private
    */
    destructor: function() {
        this._entries = [];
    },

    /////////////////////////////////////////////////////////////////////////////
    //
    // Cache protected methods
    //
    /////////////////////////////////////////////////////////////////////////////

    /**
     * Sets max.
     *
     * @method _setMax
     * @protected
     */
    _setMax: function(value) {
        // If the cache is full, make room by removing stalest element (index=0)
        var entries = this._entries;
        if(value > 0) {
            if(entries) {
                while(entries.length > value) {
                    entries.shift();
                }
            }
        }
        else {
            value = 0;
            this._entries = [];
        }
        return value;
    },

    /**
     * Gets size.
     *
     * @method _getSize
     * @protected
     */
    _getSize: function() {
        return this._entries.length;
    },

    /**
     * Gets all entries.
     *
     * @method _getEntries
     * @protected
     */
    _getEntries: function() {
        return this._entries;
    },


    /**
     * Adds entry to cache.
     *
     * @method _defAddFn
     * @param e {Event.Facade} Event Facade with the following properties:
     * <dl>
     * <dt>entry (Object)</dt> <dd>The cached entry.</dd>
     * </dl>
     * @protected
     */
    _defAddFn: function(e) {
        var entries = this._entries,
            entry   = e.entry,
            max     = this.get("max"),
            pos;

        // If uniqueKeys is true and item exists with this key, then remove it.
        if (this.get("uniqueKeys")) {
            pos = this._position(e.entry.request);
            if (LANG.isValue(pos)) {
                entries.splice(pos, 1);
            }
        }

        // If the cache at or over capacity, make room by removing stalest
        // element(s) starting at index-0.
        while (max && entries.length >= max) {
            entries.shift();
        }

        // Add entry to cache in the newest position, at the end of the array
        entries[entries.length] = entry;
    },

    /**
     * Flushes cache.
     *
     * @method _defFlushFn
     * @param e {Event.Facade} Event Facade object.
     * @protected
     */
    _defFlushFn: function(e) {
        var entries = this._entries,
            details = e.details[0],
            pos;

        //passed an item, flush only that
        if(details && LANG.isValue(details.request)) {
            pos = this._position(details.request);

            if(LANG.isValue(pos)) {
                entries.splice(pos,1);

            }
        }
        //no item, flush everything
        else {
            this._entries = [];
        }
    },

    /**
     * Default overridable method compares current request with given cache entry.
     * Returns true if current request matches the cached request, otherwise
     * false. Implementers should override this method to customize the
     * cache-matching algorithm.
     *
     * @method _isMatch
     * @param request {Object} Request object.
     * @param entry {Object} Cached entry.
     * @return {Boolean} True if current request matches given cached request, false otherwise.
     * @protected
     */
    _isMatch: function(request, entry) {
        if(!entry.expires || new Date() < entry.expires) {
            return (request === entry.request);
        }
        return false;
    },

    /**
     * Returns position of a request in the entries array, otherwise null.
     *
     * @method _position
     * @param request {Object} Request object.
     * @return {Number} Array position if found, null otherwise.
     * @protected
     */
    _position: function(request) {
        // If cache is enabled...
        var entries = this._entries,
            length = entries.length,
            i = length-1;

        if((this.get("max") === null) || this.get("max") > 0) {
            // Loop through each cached entry starting from the newest
            for(; i >= 0; i--) {
                // Execute matching function
                if(this._isMatch(request, entries[i])) {
                    return i;
                }
            }
        }

        return null;
    },

    /////////////////////////////////////////////////////////////////////////////
    //
    // Cache public methods
    //
    /////////////////////////////////////////////////////////////////////////////

    /**
     * Adds a new entry to the cache of the format
     * {request:request, response:response, cached:cached, expires:expires}.
     * If cache is full, evicts the stalest entry before adding the new one.
     *
     * @method add
     * @param request {Object} Request value.
     * @param response {Object} Response value.
     */
    add: function(request, response) {
        var expires = this.get("expires");
        if(this.get("initialized") && ((this.get("max") === null) || this.get("max") > 0) &&
                (LANG.isValue(request) || LANG.isNull(request) || LANG.isUndefined(request))) {
            this.fire("add", {entry: {
                request:request,
                response:response,
                cached: new Date(),
                expires: isDate(expires) ? expires :
            (expires ? new Date(new Date().getTime() + this.get("expires")) : null)
            }});
        }
        else {
        }
    },

    /**
     * Flushes cache.
     *
     * @method flush
     */
    flush: function(request) {
        this.fire("flush", { request: (LANG.isValue(request) ? request : null) });
    },

    /**
     * Retrieves cached object for given request, if available, and refreshes
     * entry in the cache. Returns null if there is no cache match.
     *
     * @method retrieve
     * @param request {Object} Request object.
     * @return {Object} Cached object with the properties request and response, or null.
     */
    retrieve: function(request) {
        // If cache is enabled...
        var entries = this._entries,
            length = entries.length,
            entry = null,
            pos;

        if((length > 0) && ((this.get("max") === null) || (this.get("max") > 0))) {
            this.fire("request", {request: request});

            pos = this._position(request);

            if(LANG.isValue(pos)) {
                entry = entries[pos];

                this.fire("retrieve", {entry: entry});

                // Refresh the position of the cache hit
                if(pos < length-1) {
                    // Remove element from its original location
                    entries.splice(pos,1);
                    // Add as newest
                    entries[entries.length] = entry;
                }

                return entry;
            }
        }
        return null;
    }
});

Y.Cache = Cache;


}, '3.13.0', {"requires": ["base"]});
YUI.add('moodle-core-popuphelp', function (Y, NAME) {

function POPUPHELP() {
    POPUPHELP.superclass.constructor.apply(this, arguments);
}

var SELECTORS = {
        CLICKABLELINKS: 'span.helptooltip > a',
        FOOTER: 'div.moodle-dialogue-ft'
    },

    CSS = {
        ICON: 'icon',
        ICONPRE: 'icon-pre'
    },
    ATTRS = {};

// Set the modules base properties.
POPUPHELP.NAME = 'moodle-core-popuphelp';
POPUPHELP.ATTRS = ATTRS;

Y.extend(POPUPHELP, Y.Base, {
    panel: null,

    initializer: function() {
        Y.one('body').delegate('click', this.display_panel, SELECTORS.CLICKABLELINKS, this);
    },

    display_panel: function(e) {
        if (!this.panel) {
            this.panel = new M.core.tooltip({
                bodyhandler: this.set_body_content,
                footerhandler: this.set_footer,
                initialheadertext: M.util.get_string('loadinghelp', 'moodle'),
                initialfootertext: ''
            });
        }

        // Call the tooltip setup.
        this.panel.display_panel(e);
    },

    /**
      * Override the footer handler to add a 'More help' link where relevant.
      *
      * @param {Object} helpobject The object returned from the AJAX call.
      */
    set_footer: function(helpobject) {
        // Check for an optional link to documentation on moodle.org.
        if (helpobject.doclink) {
            // Wrap a help icon and the morehelp text in an anchor. The class of the anchor should
            // determine whether it's opened in a new window or not.
            doclink = Y.Node.create('<a />')
                .setAttrs({
                    'href': helpobject.doclink.link
                })
                .addClass(helpobject.doclink['class']);
            helpicon = Y.Node.create('<img />')
                .setAttrs({
                    'src': M.util.image_url('docs', 'core')
                })
                .addClass(CSS.ICON)
                .addClass(CSS.ICONPRE);
            doclink.appendChild(helpicon);
            doclink.appendChild(helpobject.doclink.linktext);

            // Set the footerContent to the contents of the doclink.
            this.set('footerContent', doclink);
            this.bb.one(SELECTORS.FOOTER).show();
        } else {
            this.bb.one(SELECTORS.FOOTER).hide();
        }
    }
});
M.core = M.core || {};
M.core.popuphelp = M.core.popuphelp || null;
M.core.init_popuphelp = M.core.init_popuphelp || function(config) {
    // Only set up a single instance of the popuphelp.
    if (!M.core.popuphelp) {
        M.core.popuphelp = new POPUPHELP(config);
    }
    return M.core.popuphelp;
};


}, '@VERSION@', {"requires": ["moodle-core-tooltip"]});
