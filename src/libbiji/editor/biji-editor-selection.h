/*
 * e-editor-selection.h
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the program; if not, see <http://www.gnu.org/licenses/>
 *
 */

#ifndef E_EDITOR_SELECTION_H
#define E_EDITOR_SELECTION_H

#include <glib-object.h>
#include <webkit/webkit.h>

/* Standard GObject macros */
#define E_TYPE_EDITOR_SELECTION \
	(e_editor_selection_get_type ())
#define E_EDITOR_SELECTION(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST \
	((obj), E_TYPE_EDITOR_SELECTION, EEditorSelection))
#define E_EDITOR_SELECTION_CLASS(cls) \
	(G_TYPE_CHECK_CLASS_CAST \
	((cls), E_TYPE_EDITOR_SELECTION, EEditorSelectionClass))
#define E_IS_EDITOR_SELECTION(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE \
	((obj), E_TYPE_EDITOR_SELECTION))
#define E_IS_EDITOR_SELECTION_CLASS(cls) \
	(G_TYPE_CHECK_CLASS_TYPE \
	((cls), E_TYPE_EDITOR_SELECTION))
#define E_EDITOR_SELECTION_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS \
	((obj), E_TYPE_EDITOR_SELECTION, EEditorSelectionClass))

G_BEGIN_DECLS

typedef struct _EEditorSelection EEditorSelection;
typedef struct _EEditorSelectionClass EEditorSelectionClass;
typedef struct _EEditorSelectionPrivate EEditorSelectionPrivate;

typedef enum {
	E_EDITOR_SELECTION_BLOCK_FORMAT_NONE = 0,
	E_EDITOR_SELECTION_BLOCK_FORMAT_H1,
	E_EDITOR_SELECTION_BLOCK_FORMAT_H2,
	E_EDITOR_SELECTION_BLOCK_FORMAT_H3,
	E_EDITOR_SELECTION_BLOCK_FORMAT_H4,
	E_EDITOR_SELECTION_BLOCK_FORMAT_H5,
	E_EDITOR_SELECTION_BLOCK_FORMAT_H6,
	E_EDITOR_SELECTION_BLOCK_FORMAT_PARAGRAPH,
	E_EDITOR_SELECTION_BLOCK_FORMAT_BLOCKQUOTE,
	E_EDITOR_SELECTION_BLOCK_FORMAT_PRE,
	E_EDITOR_SELECTION_BLOCK_FORMAT_ADDRESS,
	E_EDITOR_SELECTION_BLOCK_FORMAT_UNORDERED_LIST,
	E_EDITOR_SELECTION_BLOCK_FORMAT_ORDERED_LIST,
	E_EDITOR_SELECTION_BLOCK_FORMAT_ORDERED_LIST_ROMAN,
	E_EDITOR_SELECTION_BLOCK_FORMAT_ORDERED_LIST_ALPHA,
} EEditorSelectionBlockFormat;

typedef enum {
	E_EDITOR_SELECTION_FONT_SIZE_TINY	= 1, /* The values match actual */
	E_EDITOR_SELECTION_FONT_SIZE_SMALL	= 2, /* size in <font size="X"> */
	E_EDITOR_SELECTION_FONT_SIZE_NORMAL	= 3,
	E_EDITOR_SELECTION_FONT_SIZE_BIG	= 4,
	E_EDITOR_SELECTION_FONT_SIZE_BIGGER	= 5,
	E_EDITOR_SELECTION_FONT_SIZE_LARGE	= 6,
	E_EDITOR_SELECTION_FONT_SIZE_VERY_LARGE	= 7
} EEditorSelectionFontSize;

typedef enum {
	E_EDITOR_SELECTION_ALIGNMENT_LEFT,
	E_EDITOR_SELECTION_ALIGNMENT_CENTER,
	E_EDITOR_SELECTION_ALIGNMENT_RIGHT
} EEditorSelectionAlignment;

struct _EEditorSelection {
	GObject parent;

	EEditorSelectionPrivate *priv;
};

struct _EEditorSelectionClass {
	GObjectClass parent_class;
};

GType			e_editor_selection_get_type 	(void);

EEditorSelection *	e_editor_selection_new		(WebKitWebView *parent_view);

gboolean		e_editor_selection_has_text	(EEditorSelection *selection);

gchar *			e_editor_selection_get_caret_word
							(EEditorSelection *selection);
void			e_editor_selection_replace_caret_word
							(EEditorSelection *selection,
							 const gchar *replacement);

void			e_editor_selection_set_alignment
							(EEditorSelection *selection,
							 EEditorSelectionAlignment alignment);
EEditorSelectionAlignment
			e_editor_selection_get_alignment
							(EEditorSelection *selection);

void			e_editor_selection_set_background_color
							(EEditorSelection *selection,
							 const gchar *color);
const gchar*		e_editor_selection_get_background_color
		 					(EEditorSelection *selection);

void			e_editor_selection_set_bold	(EEditorSelection *selection,
							 gboolean bold);
gboolean		e_editor_selection_get_bold	(EEditorSelection *selection);

void			e_editor_selection_set_font_name
							(EEditorSelection *selection,
							 const gchar *font_name);
const gchar *		e_editor_selection_get_font_name
							(EEditorSelection *selection);

void			e_editor_selection_set_font_size
							(EEditorSelection *selection,
							 guint font_size);
guint			e_editor_selection_get_font_size
							(EEditorSelection *selection);

void			e_editor_selection_set_font_color
							(EEditorSelection *selection,
							 const GdkRGBA *color);
void			e_editor_selection_get_font_color
							(EEditorSelection *selection,
							 GdkRGBA *color);

void			e_editor_selection_set_block_format
							(EEditorSelection *selection,
							 EEditorSelectionBlockFormat format);
EEditorSelectionBlockFormat
			e_editor_selection_get_block_format
							(EEditorSelection *selection);

gboolean		e_editor_selection_get_indented	(EEditorSelection *selection);
void			e_editor_selection_indent	(EEditorSelection *selection);
void			e_editor_selection_unindent	(EEditorSelection *selection);

void			e_editor_selection_set_italic	(EEditorSelection *selection,
							 gboolean italic);
gboolean		e_editor_selection_get_italic	(EEditorSelection *selection);

void			e_editor_selection_set_monospaced
							(EEditorSelection *selection,
							 gboolean monospaced);
gboolean		e_editor_selection_get_monospaced
							(EEditorSelection *selection);

void			e_editor_selection_set_strike_through
							(EEditorSelection *selection,
							 gboolean strike_through);
gboolean		e_editor_selection_get_strike_through
							(EEditorSelection *selection);

void			e_editor_selection_set_superscript
							(EEditorSelection *selection,
							 gboolean superscript);
gboolean		e_editor_selection_get_superscript
							(EEditorSelection *selection);

void			e_editor_selection_set_subscript
							(EEditorSelection *selection,
							 gboolean subscript);
gboolean		e_editor_selection_get_subscript
							(EEditorSelection *selection);

void			e_editor_selection_set_underline
							(EEditorSelection *selection,
							 gboolean underline);
gboolean		e_editor_selection_get_underline
							(EEditorSelection *selection);

void			e_editor_selection_unlink	(EEditorSelection *selection);

void			e_editor_selection_create_link	(EEditorSelection *selection,
							 const gchar *uri);

const gchar *		e_editor_selection_get_string	(EEditorSelection *selection);

void			e_editor_selection_replace	(EEditorSelection *selection,
							 const gchar *new_string);

void			e_editor_selection_insert_html	(EEditorSelection *selection,
							 const gchar *html_text);
void			e_editor_selection_insert_image	(EEditorSelection *selection,
							 const gchar *image_uri);
void			e_editor_selection_insert_text	(EEditorSelection *selection,
							 const gchar *plain_text);
void			e_editor_selection_wrap_lines	(EEditorSelection *selection);

G_END_DECLS

#endif /* E_EDITOR_SELECTION_H */