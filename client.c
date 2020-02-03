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
#include "cJSON.h"

//gets ip by name
char* get_ip(char* name)
{
    char* ip = calloc(20, sizeof(char));
    struct addrinfo hints, *result, *p;
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_protocol =0;
    getaddrinfo(name, NULL, &hints, &result);
    p = result;
    struct sockaddr_in *a;
    while(p != NULL) {
        a = (struct sockaddr_in*) p->ai_addr;
        strncpy(ip, inet_ntoa(a->sin_addr), 15);
        p = p->ai_next;
    }
    freeaddrinfo(result);
    return ip;
}

//function that gets the cookies
char* getCookies(char* response) {
    char *cookies = calloc(LINELEN, sizeof(char));
    char *startCookie = calloc(LINELEN, sizeof(char));
    char *responseAux;

    responseAux = response;
    int offset = 0;
    while (1) {
        if ((startCookie = strstr(responseAux, "Set-Cookie:")) == NULL) {
            break;
        }
        startCookie = startCookie + strlen("Set-Cookie:");
        responseAux = startCookie;
        strtok(responseAux, "\r\n");
        memcpy(cookies + offset, responseAux, strlen(responseAux));
        offset += strlen(responseAux);
        responseAux+=strlen(responseAux) + 1;
        if(strstr(responseAux, "Set-Cookie:") != NULL) {
            strcat(cookies, ";");
        }
        offset += 1;
    }
    return cookies;
}

//function that computes the message and sends request
char* compute_and_send(int task, char* url, cJSON* type, char* method, cJSON* value1,
        cJSON* value2, char*cookies, char*authHeader, char* ip, int port, char* strValue){
    //compute request
    int sockfd;
    char *message;
    char *response;
    char *task3Value = calloc(LINELEN, sizeof(char));
    char *valueForTask = calloc(LINELEN, sizeof(char));
    char *contentHdr = calloc(LINELEN, sizeof(char));
    //compute type
    if (type != NULL) {
        sprintf(contentHdr, "%s", type->valuestring);
    } else {
        contentHdr = NULL;
    }
    //compute params
    if (task == 2 || task == 5) {
        sprintf(valueForTask, "%s=%s&%s=%s", value1->string, value1->valuestring,
                value2->string, value2->valuestring);
    }
    if (task == 3) {
        sprintf(valueForTask, "raspuns1=omul&raspuns2=numele&%s=%s",
                value1->string, value1->valuestring);
        sprintf(task3Value,"%s",valueForTask);
    }
    if (task == 4) {
        memcpy(valueForTask, task3Value, strlen(task3Value));
    }
    if (task == 6) {
        sprintf(valueForTask, "%s", strValue);
    }
    //check method
    if (strcmp(method, "POST") == 0) {
        message = compute_post_request(ip, url, valueForTask, cookies, authHeader, contentHdr);
        printf("%s\n", message);
    } else {
        message = compute_get_request(ip, url, valueForTask, cookies, authHeader, contentHdr);
        printf("%s\n", message);
    }
    //send request
    sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
    return response;
}

int main(int argc, char *argv[]) {
    char *message;
    char *response;
    int sockfd;

    /*
        Task 1
    */

    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(IP_SERVER, "/task1/start", NULL, NULL, NULL, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    /*
      Task 2
    */

    // find where the body starts
    char *body = strchr(response, '{');
    cJSON *bodyParsed = cJSON_Parse(body);
    cJSON *url = cJSON_GetObjectItem(bodyParsed, "url");
    cJSON *method = cJSON_GetObjectItem(bodyParsed, "method");
    cJSON *type = cJSON_GetObjectItem(bodyParsed, "type");
    //get username and password
    cJSON *data = cJSON_GetObjectItem(bodyParsed, "data");
    cJSON *username = data->child;
    cJSON *password = data->child->next;
    //get cookies from header
    char *cookies = getCookies(response);
    char* response2 = compute_and_send(2, url->valuestring, type, method->valuestring,
            username, password, cookies, NULL, IP_SERVER, PORT_SERVER, NULL);

    /*
        Task 3
    */

    // find where the body starts
    char *body1 = strchr(response2, '{');
    cJSON *bodyParsed1 = cJSON_Parse(body1);
    cJSON *url1 = cJSON_GetObjectItem(bodyParsed1, "url");
    cJSON *type1 = cJSON_GetObjectItem(bodyParsed1, "type");
    cJSON *method1 = cJSON_GetObjectItem(bodyParsed1, "method");
    //get token, queryParams and id
    cJSON *data1 = cJSON_GetObjectItem(bodyParsed1, "data");
    cJSON *token = data1->child;
    cJSON *queryParams = data1->child->next;
    cJSON *id = queryParams->child;
    //get cookies from header
    char *cookies1 = getCookies(response2);
    char *authHeader = calloc(LINELEN, sizeof(char));
    sprintf(authHeader, "%s", token->valuestring);
    char* response3 = compute_and_send(3, url1->valuestring, type1, method1->valuestring,
            id, NULL, cookies1, authHeader, IP_SERVER, PORT_SERVER, NULL);

    /*
       Task 4
    */

    // find where the body starts
    char *body2 = strchr(response3, '{');
    cJSON *bodyParsed2 = cJSON_Parse(body2);
    cJSON *url2 = cJSON_GetObjectItem(bodyParsed2, "url");
    cJSON *method2 = cJSON_GetObjectItem(bodyParsed2, "method");
    cJSON *type2 = cJSON_GetObjectItem(bodyParsed2, "type");
    char *cookies2 = getCookies(response3);
    char* response4 = compute_and_send(4, url2->valuestring, type2,
            method2->valuestring, NULL, NULL, cookies2, authHeader, IP_SERVER, PORT_SERVER, NULL);

    /*
        Task 5
     */

    // find where the body starts
    char *body3 = strchr(response4, '{');
    cJSON *bodyParsed3 = cJSON_Parse(body3);
    cJSON *url3 = cJSON_GetObjectItem(bodyParsed3, "url");
    cJSON *method3 = cJSON_GetObjectItem(bodyParsed3, "method");
    cJSON *type3 = cJSON_GetObjectItem(bodyParsed3, "type");
    //get queryParams
    cJSON *data3 = cJSON_GetObjectItem(bodyParsed3, "data");
    cJSON *urlToGetFrom = data3->child;
    cJSON *queryParams1 = urlToGetFrom->next;
    cJSON *q = queryParams1->child;
    cJSON *APPID = q->next;
    cJSON *methodSend = queryParams1->next;
    char *cookies3 = getCookies(response4);
    //compute request
    //get the second part of the url that is needed
    char *str = strstr(urlToGetFrom->valuestring, "/");
    char *str1 = calloc(LINELEN, sizeof(char));
    strcpy(str1, str);
    char *ip = get_ip((char*)strtok(urlToGetFrom->valuestring, "/"));
    //get weather
    char* response5 = compute_and_send(5, str1, type3, methodSend->valuestring,
            q, APPID, NULL, NULL, ip, 80, NULL);
    //send info about weather
    char *weather = calloc(BUFLEN, sizeof(char));
    weather = strstr(response5, "{");
    compute_and_send(6, url3->valuestring, type3, method3->valuestring, NULL, NULL, cookies3,
            authHeader, IP_SERVER, PORT_SERVER, weather);

    free(message);
    return 0;
}
