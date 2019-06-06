#ifndef _RINOBOT_NAO_LIB_ROBOT_H_
#define _RINOBOT_NAO_LIB_ROBOT_H_

#include <string>
#include <vector>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <alproxies/almemoryproxy.h>
#include "GameControl/RoboCupGameControlData.h"

namespace Rinobot
{
    namespace Nao
    {
        typedef void (*VoidFunc)();

        class RobotModuleSettings{};

        /* 
            Classe base para módulos do robô
        */
        class RobotModule
        {
            public:
                virtual bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker);
                virtual bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker, RobotModuleSettings *settings);
                virtual void DestroyFromRobot();
        };

        /*
            A classe Robot representa um robô
            Esta classe é utilizada para conectar à um robô, permitindo 
            que posteriormente diversos módulos possam ser utilizados
            com o robô sem a necessidade de uma nova conexão. Quando o programa é finalizado
            todos os módulos são descartados e o robô é desconectado
        */
        class Robot
        {
            private:
                static VoidFunc onDisconnect;
                static bool exitOnStop;
                static AL::ALMemoryProxy *memory;
                static bool isRunning;
                static bool inited;
                static boost::shared_ptr<AL::ALBroker> broker; // O broker é o responsável por manter a conexão com o robô
                static std::string connection; // A string connection representa o nome da conexão com o robô
                static bool isRemote; // Esta variável indica se o robô esta com uma conexão remota
                static bool isSimulated; // Esta variável indica se o robô é um simulador
                static std::vector<RobotModule*> modules; // Neste vetor são armazenados todos os módulos carregados para a classe Robot
                static int teamColor, teamNumber, playerNumber;
                static void Init();
                static void RunHandler(int);
            public: 
                static const std::string LOCAL; // Atributo público que representa uma conexão local
                static bool Connect(int argc, char *argv[]); // Este método é utilizado para conectar um robô utilizando os parâmetros da função main
                static bool Connect(std::string, int = 9559); // Este método é utilizado para conectar um robô utilizando um ip e uma porta diretamente
                static void Disconnect(bool keep = false); // Este método faz com que o robô seja desconectado
                static bool IsConnected(); // Este método verifica se o robô esta conectado
                static bool IsRemote(); // Este método verifica se o robô esta conectado remotamente
                static bool IsSimulated(); // Este método verifica se o robô é um simulador

                static bool IsRunning();

                static void ExitOnStop(bool);
                static void OnDisconnect(VoidFunc fn);

                // Este método cria um módulo para a classe Robot
                template <class T>
                static T *CreateModule();
                template <class T>
                static T *CreateModule(RobotModuleSettings*);

                // Este método retorna o nome da conexão com o robô
                static std::string GetConnection();

                // GameControl methods
                static int GetTeamColor();
                static int GetTeamNumber();
                static int GetPlayerNumber();
                static void GameControl(int playerNumber, int teamNumber, int teamColor);
        };


        // Como o método de criação de módulos é statico e utiliza um template, ele deve ser definido junto da definição da classe
        template <class T>
        T *Robot::CreateModule()
        {
            // Criamos um novo módulo do tipo genérico T
            T *module = new T();
            // Invocamos o método de inicialização à partir da classe Robot
            ((RobotModule*)module)->InitFromRobot(broker);
            // Adicionamos o módulo à lista de módulos
            modules.push_back(module);
            return module; // Retornamos o novo módulo
        }

        template <class T>
        T *Robot::CreateModule(RobotModuleSettings *settings)
        {
            // Criamos um novo módulo do tipo genérico T
            T *module = new T();
            // Invocamos o método de inicialização à partir da classe Robot
            ((RobotModule*)module)->InitFromRobot(broker, settings);
            // Adicionamos o módulo à lista de módulos
            modules.push_back(module);
            return module; // Retornamos o novo módulo
        }
    }
}

#endif