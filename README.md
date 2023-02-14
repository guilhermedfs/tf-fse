# FSE-Trabalho Final
Trabalho Final da disciplina de FSE 2022/2.

Este trabalho tem por objetivo a implementação de um sistema que registra o batimento cardíaco de uma pessoa utilizando o Sensor KY039 e também o Sensor DHT11 para capturar a Temperatura e Umidade do ambiente. No trabalho, o software foi desenvolvido utilizando o ambiente de desenvolvimento espidf 5.0 com gerenciamento da extensão PlatformIO para vscode, com o uso do hardware DOIT ESP32 DEVKIT V1. O protocolo MQTT foi utilizado para envio e recebimento de mensagens com a dashboard.

## Integrantes
|Matrícula | Aluno |
| -- | -- |
| 19/0056843  |  Carlos Eduardo de Sousa Fiuza |
| 19/0026600  |  Davi Marinho da Silva Campos |
| 18/0018019  |  Guilherme Daniel Fernandes da Silva |

## Uso

1. clonar repositório
```bash
git clone https://github.com/guilhermedfs/tf-fse.git
```

2. adicionar extensão PlatformIO no vscode

3. abrir vscode > clicar na extensão PlatformIO > Home > Open Project > selecione o repositório clonado

4. conecte o hardware

5. altere as variáveis de wifi no menu_config > "Example Connection Configuration"

6. faça a build e upload e acompanhe pelo "Serial Monitor"

7. divirta-se


## Dashboard
Dashboard criada no ThingsBoard [link](http://164.41.98.25:443/dashboards/8d7024d0-aa3c-11ed-8436-09bf763c9306)

## Video apresentação
[link](https://youtu.be/vEcv7XRPmPU)
