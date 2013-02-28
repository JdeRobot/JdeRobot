#include <gtk/gtk.h>
#include <glade/glade.h>

void on_button_clicked (GtkButton *button, gpointer user_data) {
	GtkWidget *label, *entry;
	gchar *text;

	label = (GtkWidget *)g_object_get_data (G_OBJECT (button), "label");
	entry = (GtkWidget *)g_object_get_data (G_OBJECT (button), "entry");

	text = gtk_editable_get_chars (GTK_EDITABLE (entry), 0, -1);
	gtk_label_set_text (GTK_LABEL (label), text);
}

int main(int argc, char *argv[]) {
	GladeXML *xml;
	GtkWidget *label, *button, *entry;

	gtk_init(&argc, &argv);

	/* Cargar la interfaz de usuario */
	xml = glade_xml_new("hola_mundo.glade", "window1", NULL);

	button = glade_xml_get_widget (xml, "button");
	label = glade_xml_get_widget (xml, "label");
	entry = glade_xml_get_widget (xml, "entry");

//	g_assert (button != NULL);
//	g_assert (label != NULL);
//	g_assert (entry != NULL);

	g_object_set_data (G_OBJECT (button), "label", label);
	g_object_set_data (G_OBJECT (button), "entry", entry);

	/* Conectar las se�ales de la interfaz */
	//glade_xml_signal_autoconnect(xml);
	g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (on_button_clicked), NULL);

	g_object_unref (G_OBJECT (xml));

	/* Iniciar el ciclo principal */
	gtk_main();

	return 0;
}
	

