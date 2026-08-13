/**
 * @file	SocketLogger.hpp
 * @brief	Логгер, отправляющий сообщения по TCP-сокету.
 */
#ifndef SOCKETLOGGER_HPP
# define SOCKETLOGGER_HPP

# include "ILogger.hpp"
# include <string>
# include <vector>
# include <netdb.h>
# include <mutex>

/**
 * @namespace	Logger
 * @brief Пространство имен для Логгеров.
 */
namespace Logger
{

/**
 * @class	SocketLogger
 * @brief Логгер, отправляющий сообщения по TCP-сокету.
 * 
 * Поддерживает IPv4 и IPv6.
 */
class SocketLogger : public ILogger
{
private:
	int			socketFd_;
	LogLevel	minLevel_;
	std::mutex	mtx_;
public:
	/**
	 * @brief	Конструктор сокетного логгера.
	 * @param address IP-адрес или доменное имя сервера.
	 * @param port Порт сервера.
	 * @param defaultLevel Уровень логирования по умолчанию.
	 */
	SocketLogger(std::string const& address, int port, LogLevel defaultLevel);

	/**
	 * @brief	Деструктор. Закрывает сокет при уничтожении объекта.
	 */
	~SocketLogger(void);

	/**
	 * @brief	Устанавливает минимальный уровень важности.
	 * @param level Новый уровень важности.
	 */
	void				setLevel(LogLevel level) override;

	/**
	 * @brief	Отправляет сообщение по сокету с указанным уровнем.
	 * @param message Текст сообщения.
	 * @param level Уровень важности сообщения.
	 * @return true если запись успешна, false если файл не открыт или ошибка записи.
	 */
	bool				log(std::string const& message, LogLevel level) override;

	/**
	 * @brief	Отправляет сообщение по сокету с уровнем по умолчанию.
	 * @param message Текст сообщения.
	 * @return true если запись успешна, false если файл не открыт или ошибка записи.
	 */
	bool				log(std::string const& message) override;

	/**
	 * @brief	Проверяет, готов ли сокет к отправке.
	 * @return true если сокет открыт и подключён, иначе false.
	 */
	bool				isReady(void) const override;

private:
	/**
	 * @brief	Преобразует уровень логирования в строку.
	 * @param level Уровень логирования.
	 * @return Строковое представление уровня (например, "ERROR").
	 */
	std::string_view	getStrLogLevel(LogLevel level) const;

	/**
	 * @brief	Подключается к серверу, перебирая все полученные адреса.
	 * @param address IP-адрес или доменное имя сервера.
	 * @param port Порт сервера.
	 * @return true если удалось подключиться хотя бы к одному адресу.
	 */
	bool				connectToServer(std::string const& address, int port);

	/**
	 * @brief	Пытается подключиться к серверу по одному адресу.
	 * @param p Информация об адресе из getaddrinfo().
	 * @param port Порт для подключения.
	 * @return true если подключение успешно.
	 */
	bool				tryConnect(addrinfo *p, int port);

	/**
	 * @brief	Форматирует сообщение для отправки по сокету.
	 * @param message Текст сообщения.
	 * @param level Уровень важности сообщения.
	 * @return Отформатированная строка для отправки.
	 */
	std::string const	formatLogMessage(std::string const& message, LogLevel level) const;

	/**
	 * @brief	Получает список всех возможных адресов для подключения.
	 * @param address IP-адрес или доменное имя сервера.
	 * @return Пару: вектор адресов и указатель на результат getaddrinfo.
	 */
	std::pair<std::vector<addrinfo*>, addrinfo*> getAddresses(std::string const& address);
};

} // namespace Logger

#endif // SOCKETLOGGER_HPP
