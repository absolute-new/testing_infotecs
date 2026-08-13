/**
 * @file	Timer.hpp
 * @brief	Класс таймера для периодического вызова колбэков.
 */
#ifndef TIMER_HPP
# define TIMER_HPP

# include <atomic>
# include <chrono>
# include <functional>
# include <thread>

/**
 * @namespace	Stats
 * @brief Пространство имён для сервера статистики.
 */
namespace Stats
{

/**
 * @class	Timer
 * @brief Таймер, который вызывает колбэк с заданным интервалом.
 */
class Timer
{
private:
	std::thread				timerThread_;
	std::atomic<bool>		running_;
	std::chrono::seconds	interval_;
	std::function<void()>	callback_;

public:
	/**
	 * @brief	Конструктор таймера.
	 * @param interval Интервал между вызовами в секундах.
	 * @param callback Функция, которая будет вызываться каждые interval секунд.
	 */
	Timer(std::chrono::seconds interval, std::function<void()> callback);

	/**
	 * @brief	Деструктор. Останавливает таймер.
	 */
	~Timer(void);

	/**
	 * @brief	Запускает таймер.
	 */
	void	start(void);

	/**
	 * @brief	Останавливает таймер.
	 */
	void	stop(void);
};

} // namespace Stats

#endif // TIMER_HPP
