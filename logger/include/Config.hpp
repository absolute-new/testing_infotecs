/**
 * @file	Config.hpp
 * @brief	Парсер конфигурационного файла.
 */
#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <unordered_map>

/**
 * @namespace	Logger
 * @brief Пространство имен для Логгеров.
 */
namespace Logger
{

/**
 * @class	Config
 * @brief Парсер конфигурационного файла.
 * 
 * Читает файл в формате key = value.
 */
class Config
{
private:
	std::unordered_map<std::string, std::string>	data_;

public:
	/**
	 * @brief	Загружает конфигурацию из файла.
	 * @param filename Путь к файлу конфигурации.
	 * @return true если файл успешно загружен, иначе false.
	 */
	bool		load(std::string const& filename);

	/**
	 * @brief	Возвращает значение по ключу.
	 * @param key Ключ для поиска.
	 * @param defaultValue Значение по умолчанию, если ключ не найден.
	 * @return Значение или defaultValue.
	 */
	std::string	get(std::string const& key, std::string const& defaultValue = "") const;
	
	/**
	 * @brief	Возвращает значение по ключу как int.
	 * @param key Ключ для поиска.
	 * @param defaultValue Значение по умолчанию.
	 * @return Значение как int или defaultValue.
	 */
	int			getInt(std::string const& key, int defaultValue = 0) const;
};

} // namespace Logger

#endif // CONFIG_HPP
