// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-vis.
 *
 * bot2-vis is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-vis is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-vis. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_VIS_BOT_VIS_PARAM_WIDGET_H_
#define BOT2_VIS_BOT_VIS_PARAM_WIDGET_H_

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup BotGtkParamWidget BotGtkParamWidget
 * @brief GTK+ widget for creating adjustable parameters
 * @ingroup BotVisGtk
 * @include: bot_vis/bot_vis.h
 *
 * Linking: `pkg-config --libs bot2-vis`
 * @{
 */

#define BOT_GTK_TYPE_PARAM_WIDGET bot_gtk_param_widget_get_type()
#define BOT_GTK_PARAM_WIDGET(obj)                               \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), BOT_GTK_TYPE_PARAM_WIDGET, \
                              BotGtkParamWidget))
#define BOT_GTK_PARAM_WIDGET_CLASS(klass)                      \
  (G_TYPE_CHECK_CLASS_CAST((klass), BOT_GTK_TYPE_PARAM_WIDGET, \
                           BotGtkParamWidgetClass))

typedef struct _BotGtkParamWidget BotGtkParamWidget;
typedef struct _BotGtkParamWidgetClass BotGtkParamWidgetClass;

typedef enum {
  BOT_GTK_PARAM_WIDGET_DEFAULTS = 0,

  BOT_GTK_PARAM_WIDGET_ENTRY,

  // ui hints for integers
  BOT_GTK_PARAM_WIDGET_SLIDER = 1,
  BOT_GTK_PARAM_WIDGET_SPINBOX,

  // ui hints for enums
  BOT_GTK_PARAM_WIDGET_MENU,
  //    BOT_GTK_PARAM_WIDGET_RADIO,

  // ui hints for booleans
  BOT_GTK_PARAM_WIDGET_CHECKBOX,
  BOT_GTK_PARAM_WIDGET_TOGGLE_BUTTON,
} BotGtkParamWidgetUIHint;

GType bot_gtk_param_widget_get_type(void);
GtkWidget* bot_gtk_param_widget_new(void);

const char* bot_gtk_param_widget_get_type_str(BotGtkParamWidget* pw,
                                              const char* name);

int bot_gtk_param_widget_add_enum(BotGtkParamWidget* pw, const char* name,
                                  BotGtkParamWidgetUIHint ui_hints,
                                  int initial_value, const char* string1,
                                  int value1, ...) __attribute__((sentinel));

int bot_gtk_param_widget_add_enumv(BotGtkParamWidget* pw, const char* name,
                                   BotGtkParamWidgetUIHint ui_hints,
                                   int initial_value, int noptions,
                                   const char** names, const int* values);

int bot_gtk_param_widget_add_int(BotGtkParamWidget* pw, const char* name,
                                 BotGtkParamWidgetUIHint ui_hints, int min,
                                 int max, int increment, int initial_value);

int bot_gtk_param_widget_add_text_entry(BotGtkParamWidget* pw, const char* name,
                                        BotGtkParamWidgetUIHint ui_hints,
                                        const char* initial_value);

int bot_gtk_param_widget_add_double(BotGtkParamWidget* pw, const char* name,
                                    BotGtkParamWidgetUIHint ui_hints,
                                    double min, double max, double increment,
                                    double initial_value);

int bot_gtk_param_widget_add_booleans(BotGtkParamWidget* pw,
                                      BotGtkParamWidgetUIHint ui_hints,
                                      const char* name1, int initially_checked1,
                                      ...) __attribute__((sentinel));

int bot_gtk_param_widget_add_buttons(BotGtkParamWidget* pw, const char* name1,
                                     ...) __attribute__((sentinel));

void bot_gtk_param_widget_add_separator(BotGtkParamWidget* pw,
                                        const char* text);

int bot_gtk_param_widget_get_int(BotGtkParamWidget* pw, const char* name);

double bot_gtk_param_widget_get_double(BotGtkParamWidget* pw, const char* name);

const gchar* bot_gtk_param_widget_get_text_entry(BotGtkParamWidget* pw,
                                                 const char* name);

int bot_gtk_param_widget_get_bool(BotGtkParamWidget* pw, const char* name);

int bot_gtk_param_widget_get_enum(BotGtkParamWidget* pw, const char* name);

const char* bot_gtk_param_widget_get_enum_str(BotGtkParamWidget* pw,
                                              const char* name);

void bot_gtk_param_widget_set_int(BotGtkParamWidget* pw, const char* name,
                                  int val);

void bot_gtk_param_widget_set_double(BotGtkParamWidget* pw, const char* name,
                                     double val);

void bot_gtk_param_widget_set_bool(BotGtkParamWidget* pw, const char* name,
                                   int val);

void bot_gtk_param_widget_set_enum(BotGtkParamWidget* pw, const char* name,
                                   int val);

void bot_gtk_param_widget_set_enabled(BotGtkParamWidget* pw, const char* name,
                                      int enabled);

void bot_gtk_param_widget_load_from_key_file(BotGtkParamWidget* pw,
                                             GKeyFile* keyfile,
                                             const char* group_name);

void bot_gtk_param_widget_save_to_key_file(BotGtkParamWidget* pw,
                                           GKeyFile* keyfile,
                                           const char* group_name);

int bot_gtk_param_widget_modify_int(BotGtkParamWidget* pw, const char* name,
                                    int min, int max, int increment, int value);

int bot_gtk_param_widget_modify_double(BotGtkParamWidget* pw, const char* name,
                                       double min, double max, double increment,
                                       double value);

int bot_gtk_param_widget_modify_enum(BotGtkParamWidget* pw, const char* name,
                                     const char* label, const int value);

void bot_gtk_param_widget_clear_enum(BotGtkParamWidget* pw, const char* name);

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // BOT2_VIS_BOT_VIS_PARAM_WIDGET_H_
