#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include <json-c/json.h>
typedef struct rsp {
    char* enunt;
    char* url;
    char* method;
    char* type;
    char* data;
    char* username;
    char* password;
} rsp;
int main(int argc, char *argv[])
{
    int i;
    char *message;
    char *response;
    int sockfd;

    /*
        Task 1
    */
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(IP_SERVER, "/task1/start", NULL);
    printf("%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    //parse the response
    struct json_object *parsed_json;
    struct json_object *enunt;
    struct json_object *url;
    struct json_object *method;
    struct json_object *type;
    struct json_object *data;
    struct json_object *username;
    struct json_object *password;

    parsed_json = json_tokener_parse(response);
    json_object_object_get_ex(parsed_json, "enunt", &enunt);
    json_object_object_get_ex(parsed_json, "url", &url);
    json_object_object_get_ex(parsed_json, "method", &method);
    json_object_object_get_ex(parsed_json, "type", &type);
    json_object_object_get_ex(parsed_json, "data", &data);
    json_object_object_get_ex(parsed_json, "username", &username);
    json_object_object_get_ex(parsed_json, "password", &password);

    json_object_get_string(enunt);
    json_object_get_string(url);
    json_object_get_string(method);
    json_object_get_string(type);
    json_object_get_string(data);
    json_object_get_string(username);
    json_object_get_string(password);


    /*
        Ex 1: Get videos
    */
   /* sockfd = open_connection(IP_SERVER, 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(IP_SERVER, "/videos", NULL);
    printf("%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);*/

    /*
        EX 2.1: Add video
    */
    /*sockfd = open_connection(IP_SERVER, 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request(IP_SERVER, "/videos", "id=2&name=strlentest");
    printf("%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);*/


     /*
        Ex 2.2 Verificam noua colectie de videoclipuri
    */

    /* sockfd = open_connection(IP_SERVER, 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(IP_SERVER, "/videos", NULL);
    printf("%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);*/

    /*
        Ex 3 Autentificare
    */
/*
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request(IP_SERVER, "/task2/login", "username=lOeppofg&password=sgQCvA7J");
    printf("%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
*/
    /*sockfd = open_connection(IP_SERVER, 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(IP_SERVER, "/weather/key", NULL);
    printf("%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);*/

    /*sockfd = open_connection("82.196.7.246", 80, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("api.openweathermap.org", "/data/2.5/weather", "q=bucharest&APPID=80f21fc11662726c0c3c8c5512d89b3a");
    printf("%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
*/

    free(message);
    return 0;
}
