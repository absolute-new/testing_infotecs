/**
 * @file	LoggerConfig.hpp
 * @brief	Конфигурация клиентского приложения.
 */
#ifndef LOGGERCONFIG_HPP
# define LOGGERCONFIG_HPP

# include <string>
# include "ILogger.hpp"

namespace Logger
{

/**
 * @struct	LoggerConfig
 * @brief Конфигурация клиентского приложения.
 */
struct LoggerConfig
{
	std::string	logFile;				/*< Путь к файлу лога (используется, если useSocket == false). */
	std::string	address = "127.0.0.1";	/*< IP-адрес или доменное имя сервера (для сокетного режима). */
	int			port = 9000;			/*< Порт сервера (для сокетного режима). */
	LogLevel	level;					/*< Уровень логирования по умолчанию. */
	bool		useSocket = false;		/*< Если true — отправлять логи по сокету, иначе в файл. */
};

}

#endif // LOGGERCONFIG_HPP
