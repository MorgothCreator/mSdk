/*
 * httpd.h
 *
 *  Created on: Dec 1, 2016
 *      Author: John Smith
 */

#ifndef LIB_APP_HTTPD_HTTPD_H_
#define LIB_APP_HTTPD_HTTPD_H_


/**
 * Initialize the httpd: set up a listening PCB and bind it to the defined port
 */
void httpd_init(unsigned short port);

#endif /* LIB_APP_HTTPD_HTTPD_H_ */
