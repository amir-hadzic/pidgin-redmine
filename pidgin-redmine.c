#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#ifndef PURPLE_PLUGINS
#	define PURPLE_PLUGINS
#endif

#include <glib.h>
#include <string.h>

#define PLUGIN_ID "gtk-amir.hadzic-redmine"
#define PLUGIN_AUTHOR "Amir Hadzic <amir.hadzic@randomshouting.com>"

#define PLUGIN_PREF_URL "/plugins/core/" PLUGIN_ID "/redmineUrl"

// Why this is here is better explained on the pidgin plugin how-to wiki
#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

#include "cmds.h"
#include "debug.h"
#include "notify.h"
#include "plugin.h"
#include "version.h"
#include "prefs.h"

static PurplePlugin *this_plugin = NULL;

static PurpleCmdId  project_command_id,
                    issue_command_id;

static PurpleCmdRet
project_cb(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar **errror, void *data)
{
    purple_debug_misc(PLUGIN_ID, "project_cb called: %s\n", args[0]);

    // todo: implement project command

    return PURPLE_CMD_RET_OK;
}

static PurpleCmdRet
issue_cb(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar **errror, void *data)
{
    purple_debug_misc(PLUGIN_ID, "issue_cb called: %s\n", args[0]);

    // todo: implement issue command

    return PURPLE_CMD_RET_OK;
}

static gboolean
receiving_im_msg( PurpleAccount *account, char **sender, char **message,
                    PurpleConversation *conv, PurpleMessageFlags *flags)
{
    if (conv == NULL)
        conv = purple_conversation_new(PURPLE_CONV_TYPE_IM, account, *sender);

    GError *regex_error = NULL;
    GRegex *regex = g_regex_new("#(\\d+)", G_REGEX_OPTIMIZE, 0, &regex_error);

    gchar *url = purple_prefs_get_string(PLUGIN_PREF_URL);
    gchar *replace = g_strdup_printf("<a href=\"%s/issues/\\g<1>\">\\0</a>", url);

    *message = g_regex_replace(regex, *message, strlen(*message), 0,
    								replace, G_REGEX_MATCH_NOTEMPTY, &regex_error);

    g_regex_unref(regex);
    g_free(replace);
    g_free(regex_error);

    return FALSE;
}

static gboolean
plugin_load(PurplePlugin *plugin) {
    gchar *project_help = NULL;
    gchar *issue_help = NULL;

    this_plugin = plugin;

    // This command is used to print out the information about a project
    // inside the chat window so that other participants can see it.
    project_help = "project &lt;Project ID&gt;: Shows project information.";
    project_command_id = purple_cmd_register(
        "project",
        "w",
        PURPLE_CMD_P_DEFAULT,
        PURPLE_CMD_FLAG_IM,
        PLUGIN_ID,
        project_cb,
        project_help,
        NULL
    );

    // This command is used to print the status of an issue and the link
    // to its details.
    issue_help = "issue &lt;ID&gt;: Shows issue information.";
    issue_command_id = purple_cmd_register(
        "issue",
        "w",
        PURPLE_CMD_P_DEFAULT,
        PURPLE_CMD_FLAG_IM,
        PLUGIN_ID,
        issue_cb,
        issue_help,
        NULL
    );

    purple_signal_connect
    (
        purple_conversations_get_handle(),
        "receiving-im-msg",
        this_plugin,
        PURPLE_CALLBACK(receiving_im_msg),
        NULL
    );


    return TRUE;
}

static PurplePluginPrefFrame *
plugin_config(PurplePlugin *plugin)
{
	PurplePluginPrefFrame *frame;
	PurplePluginPref *ppref;

	frame = purple_plugin_pref_frame_new();

	ppref = purple_plugin_pref_new_with_name_and_label(
				"/plugins/core/" PLUGIN_ID "/redmineUrl",
				"Url:"
			);

	purple_plugin_pref_frame_add(frame, ppref);

	return frame;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
    purple_cmd_unregister(project_command_id);
    purple_cmd_unregister(issue_command_id);

    return TRUE;
}

static PurplePluginUiInfo prefs_info = {
	plugin_config,
	0,   /* page_num (Reserved) */
	NULL, /* frame (Reserved) */
	/* Padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,
    PLUGIN_ID,
    "Redmine",
    "0.1",
    "This is a plugin for the Redmine project management tool.",
    "This is a plugin for the Redmine project management tool.",
    PLUGIN_AUTHOR,
    "http://randomshouting.com",
    plugin_load,
    plugin_unload,
    NULL,
    NULL,
    NULL,
    &prefs_info,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

static void
init_plugin(PurplePlugin *plugin)
{
	purple_prefs_add_none("/plugins/core/" PLUGIN_ID);
	purple_prefs_add_string(PLUGIN_PREF_URL, "http://redmine.randomshouting.com");
}

PURPLE_INIT_PLUGIN(this_plugin, init_plugin, info)
