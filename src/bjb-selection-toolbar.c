/*
 * Copyright © 2012 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */


#include "config.h"

#include <clutter-gtk/clutter-gtk.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "bjb-main-view.h"
#include "bjb-selection-toolbar.h"

enum
{
  PROP_0,
  PROP_ACTOR,
  PROP_BJB_SELECTION,
  PROP_BJB_MAIN_VIEW,
  NUM_PROPERTIES
};

static GParamSpec *properties[NUM_PROPERTIES] = { NULL, };

struct _BjbSelectionToolbarPrivate
{
  /* check */
  GHashTable         *item_listeners;
  GtkWidget          *toolbar_collection;
  GtkWidget          *toolbar_favorite;
  GtkWidget          *toolbar_open;
  GtkWidget          *toolbar_print;
  GtkWidget          *toolbar_trash;
  gboolean           inside_refresh;

  /* sure */
  BjbMainView        *view ;
  ClutterActor       *actor ;
  GtkWidget          *widget ;
  GdMainView         *selection ;
  ClutterActor       *parent_actor;
  ClutterConstraint  *width_constraint ;

  /* misc gtk */
  GtkToolItem        *left_group;
  GtkToolItem        *right_group;
  GtkToolItem        *separator;
  GtkWidget          *left_box;
  GtkWidget          *right_box;
};

G_DEFINE_TYPE (BjbSelectionToolbar, bjb_selection_toolbar, G_TYPE_OBJECT);

static void
bjb_selection_toolbar_fade_in (BjbSelectionToolbar *self)
{
  g_message("bjb selection toolbar fade in");

  BjbSelectionToolbarPrivate *priv = self->priv;
  guint8 opacity;

  opacity = clutter_actor_get_opacity (priv->actor);
  
  if (opacity != 0)
    return;

  clutter_actor_show (priv->actor);
  clutter_actor_animate (priv->actor,
                         CLUTTER_EASE_OUT_QUAD,300,
                         "opacity",255,
                         NULL);
}


static void
bjb_selection_toolbar_fade_out (BjbSelectionToolbar *self)
{
  ClutterAnimation *animation;
  BjbSelectionToolbarPrivate *priv = self->priv;

  animation = clutter_actor_animate (priv->actor, CLUTTER_EASE_OUT_QUAD, 300, "opacity", 0, NULL);
  g_signal_connect_swapped (animation, "completed", G_CALLBACK (clutter_actor_hide), priv->actor);
}


static void
bjb_selection_toolbar_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
  BjbSelectionToolbar *self = BJB_SELECTION_TOOLBAR (user_data);

  if (response_id != GTK_RESPONSE_OK)
    return;

  gtk_widget_destroy (GTK_WIDGET (dialog));
  bjb_selection_toolbar_fade_in (self);
}


static void
bjb_selection_toolbar_collection_clicked (GtkButton *button, gpointer user_data)
{
}


static gboolean
bjb_selection_toolbar_disconnect_listeners_foreach (gpointer key, gpointer value, gpointer user_data)
{
  g_signal_handler_disconnect (value, GPOINTER_TO_UINT (key));
  return TRUE;
}


static void
bjb_selection_toolbar_favorite_clicked (GtkButton *button, gpointer user_data)
{
  /* TODO */
}


static void
bjb_selection_toolbar_open_clicked (GtkButton *button, gpointer user_data)
{
  /* TODO */
}


static void
bjb_selection_toolbar_print_clicked (GtkButton *button, gpointer user_data)
{
  /* TODO */
}


static void
bjb_selection_toolbar_set_item_visibility (BjbSelectionToolbar *self)
{
  BjbSelectionToolbarPrivate *priv = self->priv;
  GList *apps = NULL;
  GList *l;
  GList *selection;
  gboolean show_favorite = TRUE;
  gboolean show_open = TRUE;
  gboolean show_print = TRUE;
  gboolean show_trash = TRUE;
  gchar *open_label;
  guint fav_count = 0;
  guint apps_length;
  guint sel_length;

  g_warning ("bjb selection toolbar set item visibility");

  priv->inside_refresh = TRUE;

  //selection = bjb_selection_controller_get_selection (priv->sel_cntrlr);
  //for (l = g_list_first (selection); l != NULL; l = g_list_next (l))
  //  {
      /*PhotosBaseItem *item;
      const gchar *default_app_name;
      const gchar *urn = (gchar *) l->data;

      item = PHOTOS_BASE_ITEM (photos_base_manager_get_object_by_id (priv->item_mngr, urn));

      if (photos_base_item_is_favorite (item))
        fav_count++;

      default_app_name = photos_base_item_get_default_app_name (item);
      if (default_app_name != NULL && g_list_find (apps, default_app_name) == NULL)
        apps = g_list_prepend (apps, (gpointer) g_strdup (default_app_name));

      show_trash = show_trash && photos_base_item_can_trash (item);
      show_print = show_print && !photos_base_item_is_collection (item);*/
      show_trash = TRUE ;
      show_print = TRUE ;
   // }

  //sel_length = g_list_length (selection);
  //show_favorite = show_favorite && ((fav_count == 0) || (fav_count == sel_length));

  //apps_length = g_list_length (apps);
  //show_open = (apps_length > 0);
  show_open = TRUE ;

  /*if (sel_length > 1)
    show_print = FALSE; */

  if (apps_length == 1)
    /* Translators: this is the Open action in a context menu */
    open_label = g_strdup_printf (_("Open with %s"), (gchar *) apps->data);
  else
    /* Translators: this is the Open action in a context menu */
    open_label = g_strdup (_("Open"));

  gtk_widget_set_tooltip_text (priv->toolbar_open, open_label);
  g_free (open_label);
  g_list_free_full (apps, g_free);

  if (show_favorite)
    {
      GtkStyleContext *context;
      gchar *favorite_label;

      context = gtk_widget_get_style_context (priv->toolbar_favorite);

      if (fav_count == sel_length)
        {
          favorite_label = g_strdup (_("Remove from favorites"));
          gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->toolbar_favorite), TRUE);
          gtk_style_context_add_class (context, "documents-favorite");
        }
      else
        {
          favorite_label = g_strdup (_("Add to favorites"));
          gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->toolbar_favorite), FALSE);
          gtk_style_context_remove_class (context, "documents-favorite");
        }

      gtk_widget_reset_style (priv->toolbar_favorite);
      gtk_widget_set_tooltip_text (priv->toolbar_favorite, favorite_label);
      g_free (favorite_label);
    }

  gtk_widget_set_visible (priv->toolbar_print, show_print);
  gtk_widget_set_visible (priv->toolbar_trash, show_trash);
  gtk_widget_set_visible (priv->toolbar_open, show_open);
  gtk_widget_set_visible (priv->toolbar_favorite, show_favorite);

  priv->inside_refresh = FALSE; 
}


static void
bjb_selection_toolbar_set_item_listeners (BjbSelectionToolbar *self, GList *selection)
{
/*
  BjbSelectionToolbarPrivate *priv = self->priv;
  GList *l;

  g_hash_table_foreach_remove (priv->item_listeners, bjb_selection_toolbar_disconnect_listeners_foreach, NULL);

  for (l = g_list_first (selection); l != NULL; l = g_list_next (l))
    {
      GObject *object;
      gchar *urn = (gchar *) l->data;
      gulong id;

      object = photos_base_manager_get_object_by_id (priv->item_mngr, urn);
      id = g_signal_connect_swapped (object,
                                     "info-updated",
                                     G_CALLBACK (bjb_selection_toolbar_set_item_visibility),
                                     self);
      g_hash_table_insert (priv->item_listeners, GUINT_TO_POINTER (id), g_object_ref (object));
    }
*/
}


static void
bjb_selection_toolbar_selection_changed (GdMainView *view, gpointer user_data)
{
  
  BjbSelectionToolbar *self = BJB_SELECTION_TOOLBAR (user_data);
  BjbSelectionToolbarPrivate *priv = self->priv;

  GList *selection;

/*
  if (!bjb_selection_controller_get_selection_mode (priv->sel_cntrlr))
    return;
*/

  selection = gd_main_view_get_selection(view);
  //bjb_selection_toolbar_set_item_listeners (self, selection);

  if (g_list_length (selection) > 0)
  {
    bjb_selection_toolbar_set_item_visibility (self);
    bjb_selection_toolbar_fade_in (self);
  }
  
  else
    bjb_selection_toolbar_fade_out (self);
}


/*
static void
bjb_selection_toolbar_selection_mode_changed (PhotosSelectionController *sel_cntrlr,
                                                 gboolean mode,
                                                 gpointer user_data)
{
  BjbSelectionToolbar *self = BJB_SELECTION_TOOLBAR (user_data);

  if (mode)
    bjb_selection_toolbar_selection_changed (sel_cntrlr, self);
  else
    bjb_selection_toolbar_fade_out (self);
}*/


static void
bjb_selection_toolbar_dispose (GObject *object)
{
  BjbSelectionToolbar *self = BJB_SELECTION_TOOLBAR (object);
  BjbSelectionToolbarPrivate *priv = self->priv;

  if (priv->item_listeners != NULL)
    {
      g_hash_table_unref (priv->item_listeners);
      priv->item_listeners = NULL;
    }

  /*g_clear_object (&priv->item_mngr);

  if (priv->sel_cntrlr != NULL)
    {
      g_object_unref (priv->sel_cntrlr);
      priv->sel_cntrlr = NULL;
    }
  */

  G_OBJECT_CLASS (bjb_selection_toolbar_parent_class)->dispose (object);
}


static void
bjb_selection_toolbar_init (BjbSelectionToolbar *self)
{
  BjbSelectionToolbarPrivate *priv;
  GtkWidget                  *bin;
  GtkWidget                  *image;
  GtkStyleContext            *context;
  GdkRGBA                     color = {0.0, 0.0, 0.0, 0.0};

  self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, BJB_TYPE_SELECTION_TOOLBAR, BjbSelectionToolbarPrivate);
  priv = self->priv;

  priv->widget = gtk_toolbar_new ();
  gtk_toolbar_set_show_arrow (GTK_TOOLBAR (priv->widget), FALSE);
  gtk_toolbar_set_icon_size (GTK_TOOLBAR (priv->widget), GTK_ICON_SIZE_LARGE_TOOLBAR);
  context = gtk_widget_get_style_context (priv->widget);
  gtk_style_context_add_class (context, "osd");

  priv->actor = gtk_clutter_actor_new_with_contents (priv->widget);
  clutter_actor_set_opacity (priv->actor, 0);
  g_object_set (priv->actor, "show-on-set-parent", FALSE, NULL);

  bin = gtk_clutter_actor_get_widget (GTK_CLUTTER_ACTOR (priv->actor));
  gtk_widget_override_background_color (bin,
                                        GTK_STATE_FLAG_NORMAL,
                                        &color);

  priv->left_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  priv->left_group = gtk_tool_item_new ();
  gtk_container_add (GTK_CONTAINER (priv->left_group), priv->left_box);
  gtk_toolbar_insert (GTK_TOOLBAR (priv->widget), priv->left_group, -1);
  gtk_widget_show_all (GTK_WIDGET (priv->left_group));

  priv->toolbar_favorite = gtk_toggle_button_new ();
  image = gtk_image_new_from_icon_name ("emblem-favorite-symbolic", GTK_ICON_SIZE_INVALID);
  gtk_image_set_pixel_size (GTK_IMAGE (image), 32);
  gtk_container_add (GTK_CONTAINER (priv->toolbar_favorite), image);
  gtk_container_add (GTK_CONTAINER (priv->left_box), priv->toolbar_favorite);
  g_signal_connect (priv->toolbar_favorite,
                    "clicked",
                    G_CALLBACK (bjb_selection_toolbar_favorite_clicked),
                    self);

  priv->toolbar_print = gtk_button_new ();
  image = gtk_image_new_from_icon_name ("printer-symbolic", GTK_ICON_SIZE_INVALID);
  gtk_image_set_pixel_size (GTK_IMAGE (image), 32);
  gtk_container_add (GTK_CONTAINER (priv->toolbar_print), image);
  gtk_widget_set_tooltip_text (GTK_WIDGET (priv->toolbar_print), _("Print"));
  gtk_container_add (GTK_CONTAINER (priv->left_box), priv->toolbar_print);
  g_signal_connect (priv->toolbar_print,
                    "clicked",
                    G_CALLBACK (bjb_selection_toolbar_print_clicked),
                    self);

  priv->separator = gtk_separator_tool_item_new ();
  gtk_separator_tool_item_set_draw (GTK_SEPARATOR_TOOL_ITEM (priv->separator), FALSE);
  gtk_widget_set_visible (GTK_WIDGET (priv->separator), TRUE);
  gtk_tool_item_set_expand (priv->separator, TRUE);
  gtk_toolbar_insert (GTK_TOOLBAR (priv->widget), priv->separator, -1);

  priv->right_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  priv->right_group = gtk_tool_item_new ();
  gtk_container_add (GTK_CONTAINER (priv->right_group), priv->right_box);
  gtk_toolbar_insert (GTK_TOOLBAR (priv->widget), priv->right_group, -1);
  gtk_widget_show_all (GTK_WIDGET (priv->right_group));

  priv->toolbar_collection = gtk_button_new ();
  image = gtk_image_new_from_icon_name ("list-add-symbolic", GTK_ICON_SIZE_INVALID);
  gtk_image_set_pixel_size (GTK_IMAGE (image), 32);
  gtk_container_add (GTK_CONTAINER (priv->toolbar_collection), image);
  gtk_widget_set_tooltip_text (GTK_WIDGET (priv->toolbar_collection), _("Organize"));
  gtk_container_add (GTK_CONTAINER (priv->right_box), priv->toolbar_collection);
  g_signal_connect (priv->toolbar_collection,
                    "clicked",
                    G_CALLBACK (bjb_selection_toolbar_collection_clicked),
                    self);

  priv->toolbar_trash = gtk_button_new ();
  image = gtk_image_new_from_icon_name ("user-trash-symbolic", GTK_ICON_SIZE_INVALID);
  gtk_image_set_pixel_size (GTK_IMAGE (image), 32);
  gtk_container_add (GTK_CONTAINER (priv->toolbar_trash), image);
  gtk_widget_set_tooltip_text (GTK_WIDGET (priv->toolbar_trash), _("Delete"));
  gtk_container_add (GTK_CONTAINER (priv->right_box), priv->toolbar_trash);

  priv->toolbar_open = gtk_button_new ();
  image = gtk_image_new_from_icon_name ("document-open-symbolic", GTK_ICON_SIZE_INVALID);
  gtk_image_set_pixel_size (GTK_IMAGE (image), 32);
  gtk_container_add (GTK_CONTAINER (priv->toolbar_open), image);
  gtk_container_add (GTK_CONTAINER (priv->right_box), priv->toolbar_open);
  g_signal_connect (priv->toolbar_open,
                    "clicked",
                    G_CALLBACK (bjb_selection_toolbar_open_clicked),
                    self);

  gtk_widget_show_all (priv->widget);

  //priv->item_mngr = photos_item_manager_new ();

  /*priv->sel_cntrlr = bjb_selection_controller_new ();
  g_signal_connect (priv->sel_cntrlr,
                    "selection-changed",
                    G_CALLBACK (bjb_selection_toolbar_selection_changed),
                    self);

  

  g_signal_connect (priv->sel_cntrlr,
                    "selection-mode-changed",
                    G_CALLBACK (bjb_selection_toolbar_selection_mode_changed),
                    self);*/

  clutter_actor_show(priv->actor);
}

static void
bjb_selection_toolbar_get_property (GObject  *object,
                                    guint     property_id,
                                    GValue   *value,
                                    GParamSpec *pspec)
{
  BjbSelectionToolbar *self = BJB_SELECTION_TOOLBAR (object);

  switch (property_id)
  {
    case PROP_ACTOR:
      g_value_set_object (value, self->priv->actor);
      break;
    case PROP_BJB_SELECTION:
      g_value_set_object(value, self->priv->selection);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
bjb_selection_toolbar_set_property (GObject  *object,
                                    guint     property_id,
                                    const GValue *value,
                                    GParamSpec *pspec)
{
  BjbSelectionToolbar *self = BJB_SELECTION_TOOLBAR (object);

  switch (property_id)
  {
    case PROP_ACTOR:
      self->priv->parent_actor = g_value_get_object (value);
      break;
    case PROP_BJB_SELECTION:
      self->priv->selection = g_value_get_object (value);
      break;
    case PROP_BJB_MAIN_VIEW:
      self->priv->view = g_value_get_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
bjb_selection_toolbar_notify_width (GObject *object,
                                    GParamSpec *pspec,
                                    gpointer user_data)
{
  BjbSelectionToolbar *self = BJB_SELECTION_TOOLBAR (user_data);
  BjbSelectionToolbarPrivate *priv = self->priv;
  
  gfloat offset = 300.0;
  gfloat width;

  width = clutter_actor_get_width (priv->parent_actor);
  if (width > 1000)
    offset += (width - 1000);
  else if (width < 600)
    offset -= (600 - width);

  clutter_bind_constraint_set_offset (CLUTTER_BIND_CONSTRAINT (priv->width_constraint), -1 * offset);
}

static void
bjb_selection_toolbar_constructed(GObject *obj)
{
  ClutterConstraint *constraint ;
  BjbSelectionToolbar *self = BJB_SELECTION_TOOLBAR(obj);
  BjbSelectionToolbarPrivate *priv = self->priv ;
  
  G_OBJECT_CLASS (bjb_selection_toolbar_parent_class)->constructed (obj);

  /* item(s) selected --> fade in */
  g_signal_connect (self->priv->selection,
                    "view-selection-changed", 
                    G_CALLBACK(bjb_selection_toolbar_selection_changed),
                    self);

  g_signal_connect(priv->toolbar_trash,"clicked",
                   G_CALLBACK(action_delete_selected_notes),priv->view);

  priv->width_constraint = clutter_bind_constraint_new (priv->parent_actor, CLUTTER_BIND_WIDTH, -300.0); 
  clutter_actor_add_constraint (priv->actor, priv->width_constraint);
  
  g_signal_connect (priv->actor,
                    "notify::width",
                    G_CALLBACK (bjb_selection_toolbar_notify_width),
                    self);

  constraint = clutter_align_constraint_new (priv->parent_actor, CLUTTER_ALIGN_X_AXIS, 0.50);
  clutter_actor_add_constraint (priv->actor, constraint);

  constraint = clutter_align_constraint_new (priv->parent_actor, CLUTTER_ALIGN_Y_AXIS, 0.95);
  clutter_actor_add_constraint (priv->actor, constraint);
}

static void
bjb_selection_toolbar_class_init (BjbSelectionToolbarClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->dispose = bjb_selection_toolbar_dispose;
  object_class->get_property = bjb_selection_toolbar_get_property ;
  object_class->set_property = bjb_selection_toolbar_set_property ;
  object_class->constructed = bjb_selection_toolbar_constructed ;

  properties[PROP_ACTOR] = g_param_spec_object ("actor",
                                                "Actor",
                                                "ParentActor",
                                                CLUTTER_TYPE_ACTOR,
                                                G_PARAM_READWRITE |
                                                G_PARAM_CONSTRUCT |
                                                G_PARAM_STATIC_STRINGS);

  g_object_class_install_property (object_class,PROP_ACTOR,properties[PROP_ACTOR]);

  properties[PROP_BJB_SELECTION] = g_param_spec_object ("selection",
                                                        "Selection",
                                                        "SelectionController",
                                                        GD_TYPE_MAIN_VIEW,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT |
                                                        G_PARAM_STATIC_STRINGS);

  g_object_class_install_property (object_class,PROP_BJB_SELECTION,properties[PROP_BJB_SELECTION]);

  properties[PROP_BJB_MAIN_VIEW] = g_param_spec_object ("bjbmainview",
                                                        "Bjbmainview",
                                                        "BjbMainView",
                                                        BJB_TYPE_MAIN_VIEW,
                                                        G_PARAM_READWRITE  |
                                                        G_PARAM_CONSTRUCT |
                                                        G_PARAM_STATIC_STRINGS);

  g_object_class_install_property (object_class,PROP_BJB_MAIN_VIEW,properties[PROP_BJB_MAIN_VIEW]);


  g_type_class_add_private (class, sizeof (BjbSelectionToolbarPrivate));
}


BjbSelectionToolbar *
bjb_selection_toolbar_new (ClutterActor *parent_actor,
                           GdMainView   *selection,
                           BjbMainView  *bjb_main_view)
{
  return g_object_new (BJB_TYPE_SELECTION_TOOLBAR,
                       "actor", parent_actor, 
                       "selection", selection,
                       "bjbmainview",bjb_main_view,
                       NULL);
}


ClutterActor *
bjb_selection_toolbar_get_actor (BjbSelectionToolbar *self)
{
  return self->priv->actor;
}