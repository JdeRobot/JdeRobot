#ifndef NAVIGATORCAMERA_GUI_H
#define NAVIGATORCAMERA_GUI_H

#include <gtk-2.0/gdk/gdk.h>
#include <gtk-2.0/gtk/gtk.h>
#include <gtkmm-2.4/gtkmm.h>
#include <libglademm.h>
#include <cv.h>
#include "sharer.h"

#define pi 3.14159265358979


namespace navigatorCamera {

	class Gui {
	public:

		/** Default constructor.
		 *
		 * @param sharer	Sharer instance associated to this GUI.
		 * @param gladeFile	Glade XML file path.
		 */
		Gui(Sharer *sharer, const std::string& gladeFile);

		/// Default destructor.
		virtual ~Gui();

		/** Determines whether the GUI window is visible.
		 *
		 * @return <tt>true</tt> if the GUI window is visible.
		 */
		bool isVisible();

		/** Runs a single iteration of the GUI display loop.
		 *
		 * GUI display loop is compounded by the update of the images,
		 * the update of the GTK events and the update of the filter values.
		 */
		void display();


	private:

		Sharer *sharer;	///< Pointer to the associated Api instance.

		Gtk::Main gtkmain;						///< Main Gtk application instance.
		Glib::RefPtr<Gnome::Glade::Xml> refXml;	///< Glade XML parser.
		std::string gladepath;					///< Path to the associated glade file.

		Glib::RefPtr<Gdk::GC> gc_teleoperateTrl;
		Glib::RefPtr<Gdk::GC> gc_teleoperateRtt;
		Glib::RefPtr<Gdk::Pixbuf> m_imageTrl;
		Glib::RefPtr<Gdk::Pixbuf> m_imageRtt;

		Glib::RefPtr<Gdk::Colormap> colormapTrl;
		Glib::RefPtr<Gdk::Colormap> colormapRtt;
		Gdk::Color color_white;
		Gdk::Color color_black;
		Gdk::Color color_red;
		int previous_event_x, previous_event_y;
		float prev_x, prev_y;
		int previous_event_yaw, previous_event_pitch;
		float prev_yaw, prev_pitch;

		// Private Functions
		void showPose3d();			///< Show the fields of current Pose3D in GUI.
		void teleoperateTrl();
		void teleoperateRtt();

		// Windows
		Gtk::Window *showWindow;	///< Window for show the image of RGB camera and the fields of Pose3D.
		Gtk::Window *controlWindow;	///< Window for controllers.

		// Widgets
		Gtk::Image* RGB;			///< RGB Camera image.
	        Gtk::DrawingArea *teleopAreaTrl;
	        Gtk::DrawingArea *teleopAreaRtt;

		Gtk::Entry *txtInfoX;		///< Display for 'x' value of Pose3D.
		Gtk::Entry *txtInfoY;		///< Display for 'y' value of Pose3D.
		Gtk::Entry *txtInfoZ;		///< Display for 'z' value of Pose3D.
		Gtk::Entry *txtInfoH;		///< Display for 'h' value of Pose3D.
		Gtk::Entry *txtInfoQ0;		///< Display for 'q0' value of Pose3D.
		Gtk::Entry *txtInfoQ1;		///< Display for 'q1' value of Pose3D.
		Gtk::Entry *txtInfoQ2;		///< Display for 'q2' value of Pose3D.
		Gtk::Entry *txtInfoQ3;		///< Display for 'q3' value of Pose3D.

		Gtk::Button *bttnTrlnRight;		///< Right translation button.
		Gtk::Button *bttnTrlnLeft;		///< Left translation button.
		Gtk::Button *bttnTrlnFront;		///< Front translation button.
		Gtk::Button *bttnTrlnBack;		///< Back translation button.
		Gtk::Button *bttnTrlnUp;		///< Up translation button.
		Gtk::Button *bttnTrlnDown;		///< Down translation button.

		Gtk::Button *bttnRttnRight;		///< Right rotation (Yaw angle) button.
		Gtk::Button *bttnRttnLeft;		///< Left rotation (Yaw angle) button.
		Gtk::Button *bttnRttnUp;		///< Up rotation (Pitch) button.
		Gtk::Button *bttnRttnDown;		///< Down rotation (Pitch) button.
		Gtk::Button *bttnRttnCW;		///< Clockwise rotation (Roll) button.
		Gtk::Button *bttnRttnACW;		///< Anticlockwise rotation (Roll) button.

		Gtk::SpinButton *spnbTrlnStp;	///< Translation step selector.
		Gtk::SpinButton *spnbRttnStp;	///< Rotation step selector.

		Gtk::Button *bttnRstr;		///< Restart button.

		// Handlers
		void gtk_bttnTrlnRight_onButtonClick();
		void gtk_bttnTrlnLeft_onButtonClick();
		void gtk_bttnTrlnFront_onButtonClick();
		void gtk_bttnTrlnBack_onButtonClick();
		void gtk_bttnTrlnUp_onButtonClick();
		void gtk_bttnTrlnDown_onButtonClick();

		void gtk_bttnRttnRight_onButtonClick();
		void gtk_bttnRttnLeft_onButtonClick();
		void gtk_bttnRttnUp_onButtonClick();
		void gtk_bttnRttnDown_onButtonClick();
		void gtk_bttnRttnCW_onButtonClick();
		void gtk_bttnRttnACW_onButtonClick();

		void gtk_spnbTrlnStp_valueChanged();
		void gtk_spnbRttnStp_valueChanged();

		void gtk_restart_onButtonClick();

		bool on_key_press_event(GdkEventKey* event);
	        bool on_press_teleopAreaTrl(GdkEvent * event);
	        bool on_press_teleopAreaRtt(GdkEvent * event);



	}; /* class Gui */

} /* namespace navigatorCamera */
#endif /* NAVIGATORCAMERA_GUI_H */
