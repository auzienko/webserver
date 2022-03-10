/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_status_codes.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 10:06:11 by zcris             #+#    #+#             */
/*   Updated: 2022/02/07 10:39:13 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_STATUS_CODES_HPP
#define HTTP_STATUS_CODES_HPP

#define CODE_100 "Continue";
#define CODE_101 "Switching Protocols";
#define CODE_102 "Processing";
#define CODE_103 "Early Hints";

#define CODE_200 "OK";
#define CODE_201 "Created";
#define CODE_202 "Accepted";
#define CODE_203 "Non-Authoritative Information";
#define CODE_204 "No Content";
#define CODE_205 "Reset Content";
#define CODE_206 "Partial Content";
#define CODE_207 "Multi-Status";
#define CODE_208 "Already Reported";
#define CODE_226 "IM Used";

#define CODE_300 "Multiple Choices";
#define CODE_301 "Moved Permanently";
#define CODE_302 "Found";
#define CODE_303 "See Other";
#define CODE_304 "Not Modified";
#define CODE_305 "Use Proxy";
#define CODE_307 "Temporary Redirect";
#define CODE_308 "Permanent Redirect";

#define CODE_400 "Bad Request";
#define CODE_401 "Unauthorized";
#define CODE_402 "Payment Required";
#define CODE_403 "Forbidden";
#define CODE_404 "Not Found";
#define CODE_405 "Method Not Allowed";
#define CODE_406 "Not Acceptable";
#define CODE_407 "Proxy Authentication Required";
#define CODE_408 "Request Timeout";
#define CODE_409 "Conflict";
#define CODE_410 "Gone";
#define CODE_411 "Length Required";
#define CODE_412 "Precondition Failed";
#define CODE_413 "Content Too Large";
#define CODE_414 "URI Too Long";
#define CODE_415 "Unsupported Media Type";
#define CODE_416 "Range Not Satisfiable";
#define CODE_417 "Expectation Failed";
#define CODE_418 "I'm a teapot";
#define CODE_421 "Misdirected Request";
#define CODE_422 "Unprocessable Content";
#define CODE_423 "Locked";
#define CODE_424 "Failed Dependency";
#define CODE_425 "Too Early";
#define CODE_426 "Upgrade Required";
#define CODE_428 "Precondition Required";
#define CODE_429 "Too Many Requests";
#define CODE_431 "Request Header Fields Too Large";
#define CODE_451 "Unavailable For Legal Reasons";

#define CODE_500 "Internal Server Error";
#define CODE_501 "Not Implemented";
#define CODE_502 "Bad Gateway";
#define CODE_503 "Service Unavailable";
#define CODE_504 "Gateway Timeout";
#define CODE_505 "HTTP Version Not Supported";
#define CODE_506 "Variant Also Negotiates";
#define CODE_507 "Insufficient Storage";
#define CODE_508 "Loop Detected";
#define CODE_510 "Not Extended";
#define CODE_511 "Network Authentication Required";

#endif
