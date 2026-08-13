/**
 * @file	ILogger.hpp
 * @brief	Интерфейс для всех логгеров.
 */
#ifndef ILOGGER_HPP
# define ILOGGER_HPP

# include <string>

/**
 * @namespace	Logger
 * @brief Пространство имен для Логгеров.
 */
namespace Logger
{

/**
 * @enum	LogLevel
 * @brief Уровни важности сообщений.
 * 
 * Сообщения с уровнем ниже установленного не записываются.
 */
enum class LogLevel
{
	INFO,	/*< Информационные сообщения (обычный ход работы). */
	DEBUG,	/*< Отладочные сообщения (для разработчиков). */
	ERROR	/*< Сообщения об ошибках (критические проблемы). */
};

/**
 * @class	ILogger
 * @brief Интерфейс для всех логгеров.
 * 
 * Общий контракт для записи сообщений в журнал.
 */
class ILogger
{
public:
	virtual ~ILogger() = default;

	/**
	 * @brief	Устанавливает минимальный уровень важности.
	 * @param level Новый уровень важности.
	 */
	virtual void	setLevel(LogLevel level) = 0;

	/**
	 * @brief	Записывает сообщение с указанным уровнем.
	 * @param message Текст сообщения.
	 * @param level Уровень важности сообщения.
	 * @return true если сообщение успешно записано, иначе false.
	 */
	virtual bool	log(std::string const& message, LogLevel level) = 0;

	/**
	 * @brief	Записывает сообщение с уровнем по умолчанию.
	 * @param message Текст сообщения.
	 * @return true если сообщение успешно записано, иначе false.
	 */
	virtual bool	log(std::string const& message) = 0;
	
	/**
	 * @brief	Проверяет, готов ли логгер к работе.
	 * @return true если логгер готов, иначе false.
	 */
	virtual bool	isReady(void) const = 0;
};

} //namespace Logger

#endif // ILOGGER_HPP
