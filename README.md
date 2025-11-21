## Projeto: Monitor de Qualidade do Ar com ESP32, Wokwi e MQTT (HiveMQ)

Este repositório contém um projeto de monitoramento de qualidade do ar usando **ESP32**, **sensor de gás**, **LCD I2C 16x2** e um **LED**, desenvolvido e simulado na plataforma [Wokwi](https://wokwi.com/).  
Os dados são enviados via **MQTT** para o broker público **HiveMQ** (`broker.hivemq.com`).

### Como executar o projeto no Wokwi

1. **Acesse o Wokwi**

   - Entre em [Wokwi](https://wokwi.com/) e faça login.

2. **Criar um novo projeto ESP32**

   - Clique em **"New Project"**.
   - Escolha um exemplo com **ESP32**.

3. **Importar os arquivos deste repositório**

   - Apague qualquer código padrão que o Wokwi criar.
   - Envie/importe:
     - O conteúdo de **`sketch.ino`** para o arquivo principal (`sketch.ino`) do projeto no Wokwi.
     - O arquivo **`diagram.json`**:
       - No editor do Wokwi, abra o arquivo `diagram.json`.
       - Substitua o conteúdo pelo deste repositório.

4. **Conferir o Wi-Fi e o broker MQTT**

   - O código já está configurado para:
     - **SSID**: `Wokwi-GUEST`
     - **Senha**: `""` (vazio)
     - **Broker MQTT**: `broker.hivemq.com`
     - **Porta**: `1883`
     - **Tópico**: `wokwi-airquality`

5. **Rodar a simulação**
   - Clique em **"Start the simulation"**.
   - Observe no LCD:
     - Primeiro: `Gas Level` e o valor lido do sensor.
     - Depois: `Air Quality` (boa ou ruim).
   - O LED acenderá quando a qualidade do ar for classificada como **"Bad Air Quality"**.

---
