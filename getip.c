#include "getip.h"

char * getip(char * hostname)
{
    struct hostent *h;

    struct hostent
    {
        char *h_name; /*Official name of the host.*/
        char **h_aliases; /*A NULL-terminated array of alternate names for the host.*/
        int h_addrtype; /*The type of address being returned; usually AF_INET.*/
        int h_length; /*The length of the address in bytes.*/
        char **h_addr_list; /*A zero-terminated array of network addresses for the host. 
				Host addresses are in Network Byte Order. */
    };

#define h_addr h_addr_list[0]	/*The first address in h_addr_list. */


    /* Gethosbyname returns a struct of type hostent for the given host name.
            Hostname is : - name of host
                                      - ipv4 address started by dot notation
                                      - ipv6 address in colon (or possibily dot) notation
     */

    if ((h = gethostbyname(hostname)) == NULL)
    {
        herror("gethostbyname");
        exit(1);
    }

    printf("Host name  : %s\n", h->h_name);

    /* inet_ntoa converts internet host address to IPV4
       and stors it in struct given*/
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *) h->h_addr)));

    return (char *) inet_ntoa(*((struct in_addr *) h->h_addr));
}
