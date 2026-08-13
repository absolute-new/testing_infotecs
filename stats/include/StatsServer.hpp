/**
 * @file	StatsServer.hpp
 * @brief	Сервер, собирающий статистику из сокета.
 */
#ifndef STATSSERVER_HPP
# define STATSSERVER_HPP

# include "StatsConfig.hpp"
# include "Timer.hpp"
# include <thread>
# include <string>
# include <memory>
# include <atomic>

/**
 * @namespace	Stats
 * @brief Пространство имён для сервера статистики.
 */
namespace Stats
{

/**
 * @class	StatsServer
 * @brief Сервер, собирающий статистику из сокета.
 * 
 * Принимает сообщения по TCP, считает их количество и длины,
 * и выводит статистику по N или по таймауту T.
 */
class StatsServer
{
private:
	int						port_;
	int						n_;
	int						timeoutSec_;
	int						serverSocket_;
	Stats					stats_;
	std::unique_ptr<Timer>	timer_;
	std::atomic<bool>		running_;

public:
	/**
	 * @brief	Конструктор сервера статистики.
	 * @param port Порт для прослушивания.
	 * @param n Выводить статистику после каждых N сообщений.
	 * @param timeoutSec Выводить статистику каждые T секунд.
	 */
	StatsServer(int port, int n, int timeoutSec);

	/**
	 * @brief	Деструктор. Закрывает сокет и останавливает таймер.
	 */
	~StatsServer(void);

	/**
	 * @brief	Запускает главный цикл сервера.
	 */
	void	run(void);

	/**
	 * @brief	Останавливает сервер.
	 * 
	 * Устанавливает running_ = false и пробуждает таймер.
	 */
	void	stop(void);
	
	/**
	 * @brief	Возвращает текущую статистику.
	 * @return Копия структуры Stats с текущими данными.
	 */
	Stats	getStats(void) const;

	/**
	 * @brief	Выводит текущую статистику в консоль.
	 */
	void	printStats(void) const;

private:
	/**
	 * @brief	Инициализирует сокет сервера.
	 * @return true если сокет создан и слушает.
	 */
	bool	initSocket(void);

	/**
	 * @brief	Запускает таймер для вывода статистики по T.
	 */
	void	startTimer(void);

	/**
	 * @brief	Принимает одного клиента.
	 * @return Дескриптор клиентского сокета или -1.
	 */
	int		acceptClient(void);

	/**
	 * @brief	Обрабатывает одно сообщение от клиента.
	 * @param msg Текст сообщения.
	 */
	void	processMessage(std::string const& msg);

	/**
	 * @brief	Обрабатывает одного клиента (читает сообщения в цикле).
	 * @param clientSocket Дескриптор клиентского сокета.
	 */
	void	handleClient(int clientSocket);
};

} // namespace Stats

#endif // STATSSERVER_HPP