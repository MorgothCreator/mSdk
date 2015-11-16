#ifndef PICTUREBOX_H_
#define PICTUREBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "picturebox_def.h"
#include "api/timer_api.h"
#include "3d.h"
#include "controls_definition.h"
//#include "window_def.h"
//#######################################################################################
void picturebox(tPictureBox *settings, tControlCommandData* control_comand);
tPictureBox *new_picturebox(void *ParentWindow);
bool free_picturebox(tPictureBox* settings);
void picturebox_clear(tPictureBox* settings);
//void picturebox_copy_rectangle(tPictureBox* settings, unsigned int *src_buff, signed int src_x_buff_size, signed int src_y_buff_size, signed int src_x_offset, signed int src_y_offset);
void picturebox_copy_rectangle(tPictureBox* settings, unsigned int *src_buff, unsigned int src_buff_data_offset, tRectangle *_dest_rectangle, tRectangle *_src_rectangle, signed int src_width, signed int src_height);
void picturebox_put_pixel(tPictureBox* settings, signed int X, signed int Y, unsigned int color);
void picturebox_put_horizontal_line(tPictureBox* settings, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void picturebox_put_vertical_line(tPictureBox* settings, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
void picturebox_put_circle(tPictureBox* settings, signed int x, signed int y, signed int radius, unsigned char fill, unsigned int color);
void picturebox_put_line(tPictureBox* settings, signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color);
void picturebox_put_elipse(tPictureBox* settings, signed int xc,signed int yc,signed int rx,signed int ry, unsigned char fill, unsigned int color);
void picturebox_put_triangle(tPictureBox* settings, signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char fill, unsigned int color);
void picturebox_put_string(tPictureBox* settings, print_string_properties *properties);
void picturebox_put_3d_triangle(tPictureBox* settings, _3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color);
void picturebox_put_3d_rectangle(tPictureBox* settings, _3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color);
bool picturebox_put_bitmap(tPictureBox* settings, unsigned char *file, signed int X, signed int Y, bool use_transparency, bool scale);
bool picturebox_put_fbitmap(tPictureBox* settings, char *path, signed int X, signed int Y, bool use_transparency, bool scale);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "picturebox.c"
#endif
//#######################################################################################
#endif /* BUTON_H_ */
