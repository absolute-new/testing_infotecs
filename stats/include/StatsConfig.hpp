/**
 * @file	StatsConfig.hpp
 * @brief	Структура для хранения статистики.
 */
#ifndef STATSCONFIG_HPP
# define STATSCONFIG_HPP

# include <climits>
# include <cstdint>

/**
 * @namespace	Stats
 * @brief Пространство имён для сервера статистики.
 */
namespace Stats
{

/**
 * @struct	Stats
 * @brief Статистика сообщений, полученных по сокету.
 */
struct Stats
{
	uint64_t	totalMessage = 0;		/*< Общее количество сообщений. */
	uint64_t	infoCount = 0;			/*< Количество INFO-сообщений. */
	uint64_t	debugCount = 0;			/*< Количество DEBUG-сообщений. */
	uint64_t	errorCount = 0;			/*< Количество ERROR-сообщений. */
	uint64_t	lastHourCount = 0;		/*< Сообщений за последний час. */
	uint64_t	minLength = UINT64_MAX;	/*< Минимальная длина сообщения. */
	uint64_t	maxLength = 0;			/*< Максимальная длина сообщения. */
	uint64_t	sumLengths = 0;			/*< Сумма длин всех сообщений. */
	uint64_t	lastTotal = 0;			/*< Последнее выведенное значение. */
	double		avgLength = 0.0;		/*< Средняя длина сообщения. */
};

} // namespace Stats

#endif // STATSCONFIG_HPP
