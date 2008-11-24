/**
 * Copyright (C) 2008 Felipe Contreras
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#include <glib.h>

/* libpurple */
#include <version.h>
#include <account.h>
#include <accountopt.h>
#include <plugin.h>

#include <msn-pecan/pecan_session.h>

#define _Q(x) #x
#define Q(x) _Q(x)
#define PLUGIN_ID "prpl-" Q(PLUGIN_NAME)

static GList *
status_types (PurpleAccount *account)
{
    GList *types = NULL;

    types = g_list_append (types, purple_status_type_new (PURPLE_STATUS_AVAILABLE, NULL, NULL, TRUE));

    return types;
}

static const gchar *
list_icon (PurpleAccount *a,
           PurpleBuddy *b)
{
    return "msn";
}

static void
login (PurpleAccount *account)
{
    PecanSession *session;
    
    session = pecan_session_new (purple_account_get_username (account),
                                 purple_account_get_password (account));

    {
        PurpleConnection *gc;    

        gc = purple_account_get_connection (account);
        gc->proto_data = session;
        gc->flags |= PURPLE_CONNECTION_HTML | \
                     PURPLE_CONNECTION_FORMATTING_WBFO | \
                     PURPLE_CONNECTION_NO_BGCOLOR | \
                     PURPLE_CONNECTION_NO_FONTSIZE | \
                     PURPLE_CONNECTION_NO_URLDESC;
    }

    pecan_session_connect (session, "messenger.hotmail.com", 1863);
}

static void
logout (PurpleConnection *gc)
{
    PecanSession *session;

    session = gc->proto_data;

    g_return_if_fail (session);

    pecan_session_free (session);

    gc->proto_data = NULL;
}

static gint
send_im (PurpleConnection *gc,
         const gchar *who,
         const gchar *message,
         PurpleMessageFlags flags)
{
    return 1;
}

static void
convo_closed (PurpleConnection *gc,
              const gchar *who)
{
}

static gboolean
load (PurplePlugin *plugin)
{
    return TRUE;
}

static gboolean
unload (PurplePlugin *plugin)
{
    return TRUE;
}

static PurplePluginProtocolInfo prpl_info =
{
    .list_icon = list_icon,
    .login = login,
    .close = logout,
    .send_im = send_im,
    .convo_closed = convo_closed,
    .status_types = status_types,
#if PURPLE_VERSION_CHECK(2,5,0)
    .struct_size = sizeof (PurplePluginProtocolInfo),
#endif /* PURPLE_VERSION_CHECK(2,5,0) */
};

static PurplePluginInfo plugin_info =
{
    .magic = PURPLE_PLUGIN_MAGIC,
    .major_version = PURPLE_MAJOR_VERSION,
    .minor_version = PURPLE_MINOR_VERSION,
    .type = PURPLE_PLUGIN_PROTOCOL,
    .priority = PURPLE_PRIORITY_DEFAULT,

    .id = PLUGIN_ID,
    .name = "WLM2",
    .version = VERSION,
    .summary = "WLM Protocol Plugin",
    .description = "WLM Protocol Plugin",
    .author = "Felipe Contreras <felipe.contreras@gmail.com>",
    .homepage = "http://code.google.com/p/msn-pecan/",

    .load = load,
    .unload = unload,

    .extra_info = &prpl_info,
};

G_MODULE_EXPORT gboolean
purple_init_plugin (PurplePlugin *plugin)
{
    plugin->info = &plugin_info;

    return purple_plugin_register (plugin);
}
