/**
 * @file	FileLogger.hpp
 * @brief	Реализация файлового логгера.
 */
#ifndef FILELOGGER_HPP
# define FILELOGGER_HPP

# include "ILogger.hpp"
# include <fstream>
# include <mutex>


/**
 * @namespace	Logger
 * @brief Пространство имен для Логгеров.
 */
namespace Logger
{

/**
 * @class	FileLogger
 * @brief Логгер, записывающий сообщения в текстовый файл.
 * 
 * Поддерживает фильтрацию по уровню важности.
 */
class FileLogger : public ILogger
{
private:
	std::ofstream	file_;
	LogLevel		minLevel_;
	std::mutex		mtx_;

public:
	/**
	 * @brief	Конструктор файлового логгера.
	 * @param filename Имя файла для записи логов.
	 * @param defaultLevel Уровень логирования по умолчанию.
	 */
	FileLogger(std::string const& filename, LogLevel defaultLevel);

	/**
	 * @brief	Устанавливает новый уровень важности.
	 * @param level Новый уровень важности
	 */
	void				setLevel(LogLevel level) override;

	/**
	 * @brief	Записывает сообщение в файл с указанным уровнем.
	 * @param message Текст сообщения.
	 * @param level Уровень важности сообщения.
	 * @return true если запись успешна, false если файл не открыт или ошибка записи.
	 */
	bool				log(std::string const& message, LogLevel level) override;

	/**
	 * @brief	Записывает сообщение в файл с уровнем важности по умолчанию.
	 * @param message Текст сообщения.
	 * @return true если запись успешна, false если файл не открыт или ошибка записи.
	 */
	bool				log(std::string const& message) override;

	/**
	 * @brief	Проверяет, готов ли логгер к работе.
	 * @return true если файл открыт для записи, иначе false.
	 */
	bool				isReady(void) const override;
	
private:
	/**
	 * @brief	Преобразует уровень логирования в строке представление.
	 * @param level Уровень логирования.
	 * @return Строковое представление уровня (например, "ERROR").
	 */
	std::string_view	getStrLogLevel(LogLevel level) const;
};

} // namespace Logger

#endif // FILELOGGER_HPP
