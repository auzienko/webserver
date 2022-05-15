#ifndef DEFAULT_SETTINGS_HPP
#define DEFAULT_SETTINGS_HPP

#define DEFAULT_BUFLEN 65536				// max = 65536
#define MAX_CLIENTS 512
#define DEFAULT_LISTEN_QUEUE 1024
#define TIMEOUT 60							// Время в секундах для закрытия connection по таймауту
#define CLOSE_TIME 50						// Время в мс задержки закрытия connection после
											// lastActivity. Установить на 0 для закрытия connection
											// сразу после выполнения

#endif
