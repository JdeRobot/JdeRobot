#include "imprimir.h"

void
cerrarDialogoImprimir(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
}


void
imprimirResponse (GtkWidget *widget, gint response, gpointer data)
{
	GtkPrintSettings *configImprimir;
	GtkPageSetup *configPage;
	GtkPrintJob *job;
	GtkPrinter *printer;
	gboolean resultado;
	gchar *file = (gchar*)data;


	switch(response){
		case GTK_RESPONSE_OK:
			configImprimir = gtk_print_unix_dialog_get_settings((GtkPrintUnixDialog*)widget); 
			configPage = gtk_print_unix_dialog_get_page_setup((GtkPrintUnixDialog*)widget);
			printer = gtk_print_unix_dialog_get_selected_printer((GtkPrintUnixDialog*)widget);
			job = gtk_print_job_new("Imprimir autómata", printer, configImprimir, configPage);
			resultado =  gtk_print_job_set_source_file (job, file, NULL);
			gtk_print_job_send(job, NULL, NULL, NULL);
			cerrarDialogoImprimir(widget);
			break;

		case GTK_RESPONSE_APPLY:
			break;

		case GTK_RESPONSE_CANCEL:
			cerrarDialogoImprimir(widget);
			break;
	}
}

void
dialogoImpresion(gchar *file)
{
	GtkPrintUnixDialog *windowPrint;


	windowPrint =  (GtkPrintUnixDialog *)gtk_print_unix_dialog_new("Imprimir Autómata",NULL);
	gtk_print_unix_dialog_set_manual_capabilities (windowPrint,  GtkPrintCapabilities (GTK_PRINT_CAPABILITY_PAGE_SET | GTK_PRINT_CAPABILITY_COPIES | GTK_PRINT_CAPABILITY_COLLATE | GTK_PRINT_CAPABILITY_REVERSE |  GTK_PRINT_CAPABILITY_SCALE | GTK_PRINT_CAPABILITY_GENERATE_PDF | GTK_PRINT_CAPABILITY_NUMBER_UP));

	g_signal_connect(windowPrint, "response", G_CALLBACK(imprimirResponse), file);	
	gtk_widget_show((GtkWidget*)windowPrint);
}


/* **** *** ** * VALORES MÍNIMOS Y MÁXIMOS UTILIZADOS * ** *** **** */
void
calcularRangoUtilizadoPixmap(GtkWidget *canvas, int *xMin, int *xMax, int *yMin, int *yMax)
{
	GdkColormap *cmap;
	GdkPixbuf *pixbuf;
	GdkColor *colorPixel, *colorWhite;
	int i;
	int minX, maxX;
	int minY, maxY;
	int width, height, rowstride, n_channels;
	guchar *pixels, *p;
	int maximo, x, y;
	int indiceSize = 1500;


	//Establecemos los parametros del Máx y Mín
	minX = indiceSize;
	maxX = 0;
	minY = indiceSize;
	maxY = 0;

	//Establecemos el color Rgb del blanco
	colorWhite = (GdkColor*)g_malloc(sizeof(GdkColor));
	colorWhite->red = 240;
	colorWhite->green = 235;
	colorWhite->blue = 226;
	colorPixel = (GdkColor*)g_malloc(sizeof(GdkColor));

	//Recogemos el pixbuf
	cmap =  gdk_drawable_get_colormap(((GnomeCanvas *)canvas)->layout.bin_window);
	cmap =  gdk_gc_get_colormap (((GnomeCanvas *)canvas)->pixmap_gc);

	pixbuf = gdk_pixbuf_get_from_drawable (NULL, ((GnomeCanvas *)canvas)->layout.bin_window, cmap, 0, 0, 0, 0, indiceSize, indiceSize);
	g_assert(GDK_IS_PIXBUF(pixbuf));
	
	//Recogemos los valores para sacer el pixel y los verificamos
	n_channels = gdk_pixbuf_get_n_channels (pixbuf);
	g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
	g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);

	//Sacamos las medidas del pixbuf
	width = gdk_pixbuf_get_width (pixbuf);
	height = gdk_pixbuf_get_height (pixbuf);
	rowstride = gdk_pixbuf_get_rowstride (pixbuf);

	//Sacamos la dirección de los pixeles
	pixels = gdk_pixbuf_get_pixels (pixbuf);
	p = pixels;
			
	maximo = indiceSize * indiceSize;
	for (i=0; i<maximo; i++){	
			
		// Por motivos de eficiencia --> no utilizmos --> *red = p[0];*green = p[1];*blue = p[2];*alpha = p[3];
		colorPixel->red = *p;
		colorPixel->green = *(p +1);
		colorPixel->blue = *(p +2);
		p = p + n_channels;

		if (!gdk_color_equal(colorPixel, colorWhite)){
			y = (i / indiceSize);
			x = (i % indiceSize);
			if (minX > x) {
				minX = x;
			}
			if (maxX < x){
				maxX = x;
			}
			if (minY > y){
				minY = y;
			}
			if (maxY < y){
				maxY = y;
			}	 
		}
	}

	if (minX == indiceSize) {
		minX = 0;
	}
	if (maxX == 0) {
		maxX = indiceSize;
	}
	if (minY == indiceSize) {
		minY = 0;
	}
	if (maxY == 0){
		maxY = indiceSize;
	}

	*xMin = minX;
	*xMax = maxX;
	*yMin = minY;
	*yMax = maxY;
}
GtkWidget *
can()
{
        GtkWidget *window;
        GtkWidget *canvas;
        GnomeCanvasGroup *root;

        /* inicializamos las librer&iacute;as */
    //    gnome_init("basic-canvas", "0.1", argc, argv);

        /* crear una ventana que contenga al canvas */
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      //  g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, 0);

        /* canvas en modo RGB */
        canvas = gnome_canvas_new_aa();
        gtk_container_add(GTK_CONTAINER(window), canvas);
gnome_canvas_set_scroll_region(GNOME_CANVAS(canvas), 0, 0, 1500, 1500);

        root = gnome_canvas_root(GNOME_CANVAS(canvas));

        /* poner un c&iacute;rculo, con fondo verde y borde azul */
        gnome_canvas_item_new(root,
                        gnome_canvas_ellipse_get_type(),
                        "x1", 0.0,
                        "y1", 0.0,
                        "x2", 100.0,
                        "y2", 100.0,
                        "fill_color_rgba", 0x00ff00ff,
                        "outline_color_rgba", 0x0000ffff,
                        "width_units", 3.0,
                        NULL);

        /* mostrar los widgets creados y entrar en el bucle gtk_main */
        gtk_widget_show_all(window);

	return canvas;
     
}

/* **** *** ** * GUARDAR PIXMAP EN IMAGEN * ** *** **** */
void
guardarPixmapImagenPng(GtkWidget *canvas, gchar*ruta)
{
	GdkPixbuf *buffer;
	GdkPixbuf *bufferScale;
	GdkColormap *cmap;

	GtkWidget *canvas2=can();
	gnome_canvas_update_now (GNOME_CANVAS(canvas2));
	int xMin, xMax, yMin, yMax;
	int anchura, altura;

	calcularRangoUtilizadoPixmap(canvas, &xMin, &xMax, &yMin, &yMax);

	anchura = xMax-xMin;
	altura = yMax-yMin;

	//redibujarObjetosContenidoOculto();



	cmap =  gdk_drawable_get_colormap(((GnomeCanvas *)canvas2)->layout.bin_window);
	cmap =  gdk_gc_get_colormap (((GnomeCanvas *)canvas2)->pixmap_gc);
	buffer = gdk_pixbuf_get_from_drawable (NULL,  ((GnomeCanvas *)canvas2)->layout.bin_window, cmap, xMin, yMin, 0, 0, anchura, altura); //0, 0, 0, 0,2500, 2500
	//buffer = (GdkPixbuf *)((GnomeCanvasBuf *)canvas)->buf;
GdkImage * imagen = gdk_image_get  (((GnomeCanvas *)canvas)->layout.bin_window,
                                                         0,
                                                         0,
                                                         1500,
                                                         1500);
// GdkImlibImage      *img = gdk_imlib_create_image_from_drawable(((GnomeCanvas *)canvas)->layout.bin_window, NULL, 0, 0, 1500, 1500);
//gdk_imlib_save_image_to_ppm (img, "./imagen.ppm");

//buffer = gdk_pixbuf_new(GdkColorspace colorspace,   TRUE, 8,   1500, 1500);
//gtk_layout_get_bin_window(&((GnomeCanvas *)canvas)->layout)
//((GnomeCanvas *)canvas)->layout->layout.container.widget.window
//canvas->window
	if ((altura >= 1500) || (anchura >= 1500)){
		if ((altura>= 1500) && (anchura >= 1500)){
			altura = 1500;
			anchura = 1500;
		}else if (altura >= 1500) {
			anchura = (1500*anchura)/altura;
			altura = 1500;
		} else if (anchura >= 1500) {
			altura = (1500*altura)/anchura;
			anchura = 1500;
		}
		bufferScale = gdk_pixbuf_scale_simple(buffer, anchura, altura, GDK_INTERP_HYPER); //GKD_INTERP_BILINEAR
		gdk_pixbuf_save (bufferScale, ruta, "png", NULL, "compression", "0", NULL); //quality -> jpeg
	}
	else {
		gdk_pixbuf_save (buffer, ruta, "png", NULL, "compression", "0", NULL); //quality -> jpeg
	}
	//redibujarObjetosContenido();

	//dialogoImpresion(ruta);
}


int
sp_png_write_rgba (const unsigned char *filename, const unsigned char *px, int width, int height, int rowstride)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_color_8 sig_bit;
	png_text text_ptr[3];
	png_uint_32 k;
	png_bytep *row_pointers;

	g_return_val_if_fail (filename != NULL, FALSE);

	/* open the file */

	fp = fopen ((const char*)filename, "wb");
	g_return_val_if_fail (fp != NULL, FALSE);

	/* Create and initialize the png_struct with the desired error handler
	 * functions.  If you want to use the default stderr and longjump method,
	 * you can supply NULL for the last three parameters.  We also check that
	 * the library version is compatible with the one used at compile time,
	 * in case we are using dynamically linked libraries.  REQUIRED.
	 */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return FALSE;
	}

	/* Allocate/initialize the image information data.  REQUIRED */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_write_struct(&png_ptr, NULL);
		return FALSE;
	}

	/* Set error handling.  REQUIRED if you aren't supplying your own
	 * error hadnling functions in the png_create_write_struct() call.
	 */
	if (setjmp(png_ptr->jmpbuf)) {
		/* If we get here, we had a problem reading the file */
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return FALSE;
	}

	/* set up the output control if you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* Set the image information here.  Width and height are up to 2^31,
	 * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	 * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	 * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	 * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	 * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	 * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
	 */
	png_set_IHDR(png_ptr, info_ptr,
		     width,
		     height,
		     8, /* bit_depth */
		     PNG_COLOR_TYPE_RGB_ALPHA,
		     PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_BASE,
		     PNG_FILTER_TYPE_BASE);

	/* otherwise, if we are dealing with a color image then */
	sig_bit.red = 8;
	sig_bit.green = 8;
	sig_bit.blue = 8;
	/* if the image has an alpha channel then */
	sig_bit.alpha = 8;
	png_set_sBIT(png_ptr, info_ptr, &sig_bit);

#if 0
	/* Optional gamma chunk is strongly suggested if you have any guess
	 * as to the correct gamma of the image.
	 */
//	png_set_gAMA(png_ptr, info_ptr, gamma);

#endif
	/* Optionally write comments into the image */
	text_ptr[0].key = (char*)"Software";
	text_ptr[0].text = (char*)"Sodipodi";
	text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
#if 0
	text_ptr[1].key = (char*)"Author";
	text_ptr[1].text = (char*)"Unknown";
	text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
	text_ptr[2].key = (char*)"Description";
	text_ptr[2].text = (char*)"a picture";
	text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
#endif
	png_set_text(png_ptr, info_ptr, text_ptr, 1);

	/* other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs, */
	/* note that if sRGB is present the cHRM chunk must be ignored
	 * on read and must be written in accordance with the sRGB profile */

	/* Write the file header information.  REQUIRED */
	png_write_info(png_ptr, info_ptr);

	/* Once we write out the header, the compression type on the text
	 * chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
	 * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
	 * at the end.
	 */

	/* set up the transformations you want.  Note that these are
	 * all optional.  Only call them if you want them.
	 */

	/* --- CUT --- */

	/* The easiest way to write the image (you may have a different memory
	 * layout, however, so choose what fits your needs best).  You need to
	 * use the first method if you aren't handling interlacing yourself.
	 */

	row_pointers = g_new (png_bytep, height);

	for (k = 0; k < height; k += 64) {
		int ke, kk;
		ke = MIN (k + 64, height);
		for (kk = k; kk < ke; kk++) {
			row_pointers[kk] = (png_bytep) px + kk * rowstride;
		}
		png_write_rows (png_ptr, &row_pointers[k], ke - k);
	}

	g_free (row_pointers);

	/* You can write optional chunks like tEXt, zTXt, and tIME at the end
	 * as well.
	 */

	/* It is REQUIRED to call this to finish writing the rest of the file */
	png_write_end(png_ptr, info_ptr);

	/* if you allocated any text comments, free them here */

	/* clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* close the file */
	fclose(fp);

	/* that's it */
	return TRUE;
}

//void exportar (GnomeCanvas *canvas)
//{
//GdkImlibImage *imagen;
//GdkPixmap *pixmap = gtk_pixmap_new (canvas->layout.bin_window,
//ANCHO_IMAGEN, 
//ALTO_IMAGEN, 
//gtk_widget_get_visual (GTK_WIDGET (canvas))->depth);

//imagen = gdk_imlib_create_image_from_drawable (pixmap, 
//NULL, 
//x, y, 
//width, height);

//gdk_imlib_save_image_to_ppm (imagen, "imagen.ppm");
//gdk_imlib_kill_image (imagen);
//}

void exportar2 (GnomeCanvas *canvas)
{
  GtkWidget *widget;
//gnome_canvas_paint_rect
//GdkPixmap *pixmap = gtk_pixmap_new (canvas->layout.bin_window,
//				1500, 
//				1500, 
//				-1);
GdkPixmap *pixmap = gdk_pixmap_new (canvas->layout.bin_window, 1500,1500, //IMAGE_WIDTH, IMAGE_HEIGHT,
-                                    	24);		// gtk_widget_get_visual (GTK_WIDGET(canvas))->depth);

GdkImage *   imagen =       gdk_image_new                       (GDK_IMAGE_FASTEST,
                                                         gtk_widget_get_visual (GTK_WIDGET (canvas)),
                                                         1500,
                                                         1500);
// GdkPixbuf *buffer = gdk_pixbuf_get_from_image           (NULL,
//                                                         imagen,
//                                                         gdk_gc_get_colormap (canvas->pixmap_gc),
//                                                         0, 0, 0, 0,1500, 1500);


GdkPixbuf *buffer = gdk_pixbuf_get_from_drawable          (NULL,
                                                         pixmap,
                                                         gdk_gc_get_colormap (canvas->pixmap_gc),
                                                         0, 0, 0, 0,1500, 1500);
 gdk_pixbuf_save (buffer, "./prueba.png", "png", NULL, "compression", "0", NULL);
}


