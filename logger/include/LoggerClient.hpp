/**
 * @file	LoggerClient.hpp
 * @brief	Клиентское приложение для отправки логов.
 */
#ifndef LOGGERCLIENT_HPP
# define LOGGERCLIENT_HPP

# include "LoggerConfig.hpp"
# include <memory>
# include <optional>
# include <queue>
# include <mutex>
# include <condition_variable>
# include <thread>
# include <atomic>

/**
 * @namespace	Logger
 * @brief Пространство имен для Логгеров.
 */
namespace Logger
{

/**
 * @struct	LogEntry
 * @brief Структура для хранения записи лога в очереди.
 */
struct LogEntry
{
	std::string	message;
	LogLevel	level;
};

/**
 * @class	LoggerClient
 * @brief Клиентское приложение для отправки логов.
 * 
 * Принимает ввод от пользователя и отправляет его в логгер.
 */
class LoggerClient
{
private:
	LoggerConfig				config_;
	std::unique_ptr<ILogger>	Logger_;
	std::queue<LogEntry>		messageQ_;
	std::mutex					queueMtx_;
	std::condition_variable		queueCV_;
	std::thread					writeThrd_;
	std::atomic<bool>			running_;
	bool						ready_;

public:
	/**
	 * @brief	Конструктор клиентского приложения.
	 * @param config Конфигурация приложения.
	 */
	LoggerClient(LoggerConfig const& config);
	/**
	 * @brief	Деструктор. Останавливает потоки.
	 */
	~LoggerClient(void);

	/**
	 * @brief	Запускает главный цикл ввода.
	 */
	void							run(void);
	/**
	 * @brief	Парсит строку в LogLevel.
	 * @param level Строка с уровнем.
	 * @return LogLevel или nullopt, если строка невалидна.
	 */
	static std::optional<LogLevel>	parseLogLevel(std::string const& level);
	/**
	 * @brief	Проверяет, готово ли приложение к работе.
	 * @return true если логгер готов, иначе false.
	 */
	bool							isReady(void) const;

private:
	/**
	 * @brief	Проверяет, является ли ввод командой выхода.
	 * @param input input Строка ввода.
	 * @return true если ввод == "exit".
	 */
	bool							isExit(std::string const& input) const;
	/**
	 * @brief	Преобразует уровень в строку для вывода.
	 * @param level Уровень логирования.
	 * @return Строковое представление уровня.
	 */
	std::string_view				logLevelToString(LogLevel level) const;

	/**
	 * @brief	Цикл рабочего потока, записывающий в логгер.
	 */
	void							writeLoop(void);
};

}

#endif // LOGGERCLIENT_HPP
