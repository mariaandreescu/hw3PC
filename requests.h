#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_get_request(char *host, char *url, char *url_params, char *cookies, char *headers, char* content);
char *compute_post_request(char *host, char *url, char *form_data, char *cookies, char *headers, char* content);

#endif