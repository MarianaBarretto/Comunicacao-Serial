<h1 align="center">Controle de LEDs e Display - Embarcatech 💻</h1>

<h2>Descrição</h2>

<p>
    Este projeto utiliza a Raspberry Pi Pico para controlar uma matriz de LEDs 5x5 e um display OLED SSD1306 via I2C. O sistema permite interação através de dois botões físicos, que alternam o estado dos LEDs e exibem informações no display.
</p>

<h2>Funcionalidades</h2>

<ul>
    <li>Controle de uma matriz de LEDs WS2812B 5x5</li>
    <li>Exibição de números na matriz de LEDs</li>
    <li>Uso de display OLED SSD1306 para exibição de mensagens</li>
    <li>Botões físicos para interação com os LEDs e o display</li>
 </ul>

<h2>Componentes Utilizados</h2>

<ul>
    <li><strong>Raspberry Pi Pico</strong></strong></li> 
    <li><strong>Matriz de LEDs WS2812B</strong> (5x5)</strong></li> 
    <li><strong>Display OLED SSD1306</strong> (I2C</strong></li> 
    <li><strong>Botões</strong> (2 unidades)</strong></li>    
 </ul>

 <h2>Conexões</h2>
 
<h4>LEDs:</h4>
 <ul>    
    <li>LED Verde: GPIO 11</li>
    <li>LED Azul: GPIO 12</li>
    <li>LED Vermelho: GPIO 13</li>
 </ul>
    
<h4>Botões:</h4>

 <ul>      
    <li>Botão A: GPIO 5</li>
    <li>Botão B: GPIO 6</li>
 </ul>    
    
<h4>I2C para o Display OLED:</h4>

 <ul>
    <li>SDA: GPIO 14</li>
    <li>SCL: GPIO 15</li>
 </ul>
 
 <h2>Configuração do Ambiente</h2> 
    <p>1. Instale o <strong>SDK do Raspberry Pi Pico</strong> e configure o <strong>CMake</strong>.</p>
    <p>2. Baixe e instale o <strong>Visual Studio Code</strong> e configure o <strong>Wokwi</strong> para simulação.</p>
    <p>3. Clone este repositório e navegue até a pasta do projeto.</p>
    <p>4. Compile o código e envie para a BitDogLab.</p>


<p>
    <h2>Como Usar</h2>
    1. Ligue a BitDogLab.<br>
    2. Pressione o <strong>Botão A</strong> para ligar/desligar o LED Verde e atualizar a mensagem no display.<br>
    3. Pressione o <strong>Botão B</strong> para ligar/desligar o LED Azul e atualizar a mensagem no display.<br>
    4. Utilize o Serial Monitor do VSCode para enviar os números.	
    4. A matriz de LEDs exibirá números predefinidos.
</p>

<p>
    <h2>Estrutura do Código</h2>
    - <strong>Definições</strong>: Configura pinos e periféricos.<br>
    - <strong>Funções</strong>:
        <ul>
            <li><code>matrix_rgb()</code>: Converte valores RGB.</li>
            <li><code>atualizar_display()</code>: Atualiza a tela OLED.</li>
            <li><code>desenho()</code>: Renderiza um número na matriz de LEDs.</li>
            <li><code>gpio_irq_handler()</code>: Trata interrupções dos botões.</li>
        </ul>
    - <strong><code>main()</code></strong>: Configura e inicia o sistema.
</p>

 <h2>Imagens</h2>
  
  <h4 align="center">LED Verde Ligado</h4>

  <p align="center">
  <img src="https://github.com/MarianaBarretto/Comunicacao-Serial-/blob/feature/projeto/LED%20Verde%20Ligado.jpeg" alt="LED Verde Ligado" />
  </p>

 <h4 align="center">LED Verde Desligado</h4>
 
  <p align="center">
  <img src="https://github.com/MarianaBarretto/Comunicacao-Serial-/blob/feature/projeto/LED%20Verde%20Desligado.jpeg" alt="LED Verde Desligado" />
  </p>

  <h4 align="center">LED Azul Ligado</h4>

  <p align="center">
  <img src="https://github.com/MarianaBarretto/Comunicacao-Serial-/blob/feature/projeto/LED%20Azul%20Ligado.jpeg" alt="LED Azul Ligado" />
  </p>

 <h4 align="center">LED Azul Desligado</h4>

  <p align="center">
  <img src="https://github.com/MarianaBarretto/Comunicacao-Serial-/blob/feature/projeto/LED%20Azul%20Desligado.jpeg" alt="LED Azul Desligado" />
  </p>

  <h4 align="center">Número sendo Exibido</h4>

  <p align="center">
  <img src="https://github.com/MarianaBarretto/Comunicacao-Serial-/blob/feature/projeto/N%C3%BAmero%20sendo%20exibido.jpeg" alt="LED Azul Desligado" />
  </p>
  
  <h4 align="center">>Serial Monitor</h4>

  <p align="center">
  <img src="https://github.com/MarianaBarretto/Comunicacao-Serial-/blob/feature/projeto/Serial%20Monitor.jpeg" alt="LED Azul Desligado" />
  </p>
  
 
<h2>Link do Vídeo do Projeto</h2>

<h3>Vídeo de Demostração</h3>

<p>Google Drive: https://drive.google.com/file/d/1Cou9XxzCrE80lTFcdVBZ4rDK_691J3M7</p>
<p>YouTube: https://www.youtube.com/watch?v=0MhkmSo-rVo</p>

<h4>Autor: <strong>Mariana Barretto</strong></h4>
