
/* Change endpoint address, thing name & topic based on your AWS IoT account configurations. Below values are just for reference */
#define AWSIOT_ENDPOINT_ADDRESS             "a2wez1ijy9ju6g-ats.iot.us-east-1.amazonaws.com"
#define AWSIOT_THING_NAME                   "AWS_Thing"

/* Make sure Client-ID is unique for each AWS IoT connection */
#define AWSIOT_CLIENT_ID                    "iotconsole-1557246011977-6"
#define AWSIOT_TOPIC                        "AWS_TOPIC"

/* Change device certificate and device private key based on your account */
const char SSL_CLIENTCERT_PEM[] =  "\
-----BEGIN CERTIFICATE-----\n\
MIIDWTCCAkGgAwIBAgIUJSyM97OJ1rV9+o8GFN6D0Kfgz5AwDQYJKoZIhvcNAQEL\
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDUwMjA5MTcw\
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALcnq1/QyQhFu8h1Nz4b\
6XFuA/mHO6PbqMGs3Reo/HO/XAUZHgCBFHhB5GHcVD7mEnAA6wWcyKmRqM2kVgt9\
T6KMYrVK6VhALqV13mSmvdtaaqittm92AXuP/CJKBO+9wLuhs2mzUbI/COv50aVp\
UNvlvDqiY90XpjRd4uKFPEVXtuHa2Lbvt0G3MmfvwJRr+phWS0VhMpthjGpux1hu\
8JRSXtYD1WnhQC3dvJIWUDhu792Lyw2IYQggeH5Hu0gkDoCSwMA0E6i0d6EbrOkO\
gWRM2xTHYu/uIsW4vM+ZXeTbLe5nX7ZtlPsBokMqvkmL/thURYEpZI2IljNhkThT\
fkMCAwEAAaNgMF4wHwYDVR0jBBgwFoAU4yFOCiaTEOPd68C7EZW8qz62B8IwHQYD\
VR0OBBYEFC7hAlzlp5GCwgFRq1drZ4GH/e1SMAwGA1UdEwEB/wQCMAAwDgYDVR0P\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBUqeevQk2cGwnu22uMHrdp3K4r\
NSzMneE1CVfzdgv75TejkDlG2Yspni+q3BrDmZDzmVo7JqjMeZqNv/DQDc+b1MvM\
MbaDhbP4052c+LCG1J9w6/pRy6wcri33x7CEuwR75u7wgjyA3BSjoH4R8/asF6b/\
A0Q09jH3v4/IsJyfkJKA0Z+bQ56Ch0WauYMkZyB0uRlClMV89y1zDmmM0b9RXI+u\
2y7ubqSwZ0lqnB0Rz8KmZ659b7rxM25OpTSpNKmIAj+7cJMsiZh8IA9Zo7SjoWB5\
PU7A2IVYhQsOK0zbSyzpYifr1x0QEJ3Bel87UzgJDwEMUSbtpeO7HTlL/7sj\
\n\
-----END CERTIFICATE-----\n";

const char SSL_CLIENTKEY_PEM[] = "\
-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAtyerX9DJCEW7yHU3PhvpcW4D+Yc7o9uowazdF6j8c79cBRke\
AIEUeEHkYdxUPuYScADrBZzIqZGozaRWC31PooxitUrpWEAupXXeZKa921pqqK22\
b3YBe4/8IkoE773Au6GzabNRsj8I6/nRpWlQ2+W8OqJj3RemNF3i4oU8RVe24drY\
tu+3QbcyZ+/AlGv6mFZLRWEym2GMam7HWG7wlFJe1gPVaeFALd28khZQOG7v3YvL\
DYhhCCB4fke7SCQOgJLAwDQTqLR3oRus6Q6BZEzbFMdi7+4ixbi8z5ld5Nst7mdf\
tm2U+wGiQyq+SYv+2FRFgSlkjYiWM2GROFN+QwIDAQABAoIBAAmJEy8F27GGj+Q7\
1ghxw5GYPLS9CanaQcrziP82WQ5dpBjP2fQExGAObil6IxizObqDEblVFE1fu/4Y\
1DH88CxySqFNqSBjFwqePFnZJIFLD8qAYMMkzab1NBdE60TgCCy8mm6iW1qAVaKT\
ewq1lPQd3dLfqXtVQ93uFWXdsemaNMeZrgYDZMU7TobD9cQzX2gZZ4MO4O9cDTte\
ke5ABm6xXTPAxEgXro8JeVdA1WOOHcUe/VLOnPb6YZUoOJTo9gJsuiBIByZ8sjvN\
WaICEf/PkBJX1npWxQgn6J0a+P9VG3WV06FAsl6fF2hK5oRPRqT5jA9kLhsRsUX7\
OvNNH+ECgYEA8fldMGTWI8sxlb8hxFuFfDSEQ/zczxy35DvZ/OyMDEGbOs4vWiae\
TH8k5vcNxLkX2JyVIpyxMOm4H6GMx4UBGICog4JKMW9mUm0Q+BUWNgQs7PRya8Jv\
5ycnT2nbc8OzxZwPhirixaQB18moeHMYuV7r6RrtziJB+CI97RoZOmcCgYEAwcV+\
IsqkBXXqeDCSONV54XfeD/O0aG3IOXttE+N8gC4dyele1TvC5yx3+xDQ+Zx9GzZn\
BL35t/PXDF3yCxYnPl+/R/hsxwZL/YzoYqTfryxjKk6OhsVCQbPOSxuk2NmRcIXd\
6Thin19K99asFBPW/SCO/5e77Oi9cCxpf50r68UCgYEAkzT/SneuBuyQ5+b1Zf2/\
wYY5iRsGQkHYZdM7lLFbqgEnrKjPCetAlo0rKA/YontwVGk+GWZ2LamiSCwB90el\
GQD8g7T56gU5nJ+ZmTNK5eW4Zxd3nLMXXOrm8VsXFOBUN73ve9IPT+ms7VhTU6PB\
wNRMdO3bCnKw64H5ZotgGbUCgYBiGviS6uACd8H/mi/eII81zXebwfqcSMDwAYKn\
aCzXxQGGAebmtPwoCQayEcCMMGPTDEc6SmkNbqJ5e3MZk9zIord429lPJuwAcoAR\
uYIa8bE/cUiOCX9TPpL6ygM7W2RcgRpqgwbDw/5z3rd+7tCRlhNwasVV3DxVd9bu\
KohbNQKBgH4bOvq77JQTJzdeH5dWixbxq9GqBm8ocURbeehjT7DtFcssqTPPFc5z\
Guoyn45Xd3SoOu+DrvryRWtNcQ8c24e1wnCz9fG4zP/9Y5q/ycl8lw6ptAInP12Z\
kT1gBGQ+k7DTStZPFfdBS9HLA3HmY4c0QBkFowCrbukQ74S/jnU2\
\n\
-----END RSA PRIVATE KEY-----\n";

const char SSL_CA_PEM[] = "\
-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\
rqXRfboQnoZsG4q5WTP468SQvvG5\
\n\
-----END CERTIFICATE-----\n";