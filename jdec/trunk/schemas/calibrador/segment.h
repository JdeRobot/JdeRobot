
extern void set_sorted_lines(Tcalib_volume *vol ,
		      int num_of_lines,
		      int check_lines);

extern void set_sorted_vector(Tvector *v, Tpoint2D p1, Tpoint2D p2);
extern float ang_entre_vectores(Tvector v1, Tvector v2);
extern float ang_dirigido_entre_vectores(Tpoint2D p1, Tpoint2D center);
extern int aligned_points(Tpoint2D *ref, Tpoint2D *points, int length);
