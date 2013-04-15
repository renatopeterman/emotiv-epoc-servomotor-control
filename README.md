Controle de Servomotor com Emotiv Epoc
===================

Projeto desenvolvido utilizando o headset Emotiv Epoc e Arduino para controlar Servomotores utilizando a Suite Expressiv do Emotiv Epoc.

<h3>Entendendo os arquivos:</h3>

<b>EmotivApp</b>

Esta aplicação é responsável pela comunicação entre o Epoc e o Arduino. Ela recebe e interpreta os sinais do headset utilizando o SDK do Emotiv Epoc e envia uma string para o Arduino que utilizada para identificar como o motor deve ser movimentado.
O programa foi desenvolvido em C++ utilizando o Visual Studio 2008.

<b>Multiservo</b>

Este é o código que deve ser utilizado no Arduino, ele é responsável por receber uma string composta por 4 caracteres da aplicação e então movimentar o(s) servo(s).
Os primeiros 3 caracteres são utilizados para identificar o grau de rotação do motor e o último é utilizado para identificar qual motor deve ser movimentado.

<b>Exemplo:</b> 
Se a string enviada para o Arduino for igual a "180a", isto significa que o servo "A" deve ser movido 180 graus.
Se a string for igual a "070b", isto significa que o servo "B" deve ser movimentado 70 graus, e assim por diante.

<h3>Funcionamento:</h3>

Este video mostra o projeto em funcionamento: http://www.youtube.com/watch?v=lEx4d8qCdus

É muito simples! A aplicação EmotivApp recebe e identifica as ações do Emotiv Epoc e a partir delas envia uma string para o Arduino informando qual servo e quanto o grau de movimentação do mesmo.

<h3>Ferramentas utilizadas no projeto:</h3>

<ul>
<li>Emotiv Epoc (Developer Edition)</li>
<li>Emotiv Epoc SDK 1.0.0.3</li>
<li>Arduino Duemilanove</li>
<li>Arduino IDE 0018</li>
<li>Servo Motor Futaba S3003</li>
<li>Microsoft Visual Studio 2008</li>
</ul>

<h3>Mais Informações:</h3>

Meu site pessoal: www.renatopeterman.com.br<br/>
Emotiv: www.emotiv.com<br/>
Arduino: www.arduino.cc<br/>

Emotiv Epoc Servomotor Control
===================

Project developed using Emotiv Epoc Headset (BCI) and Arduino that shows hardware interaction using the Emotiv Epoc (Brain Computer Interface) to control Servos using the Emotiv Expressiv Suite.

<h3>Understanding the files:</h3>

<b>EmotivApp</b>

This application is used to communicate with Arduino through serial communication. The application send the commando (a string) to the Arduino that is used to move the servomotor.
It was developed in C++ with Visual Studio 2008.

<b>Multiservo</b>

This is the Arduino code, It is responsible to receive the string with 4 characters and move the servo. The first 3 characters is used to identify how many degrees the servo must to move and the last character is used to identify which servo do you want to move. (see example below).

<b>Example:</b> 
If the string sended to Arduino is equals to "180a", this means that servo "A" must move 180 degrees.
If the string is equals to "070b", this means tha servo "B" must move 70 degrees and so on.

<h3>How it works:</h3>

This videos shows how the project works: http://www.youtube.com/watch?v=lEx4d8qCdus

It's very simple! The C++ application receive and identify (with Epoc SDK) the actions from Epoc headset and then send the string with which servo and how many degrees the servo must move.

<h3>Tools used in the project:</h3>

<ul>
<li>Emotiv Epoc (Developer Edition)</li>
<li>Emotiv Epoc SDK 1.0.0.3</li>
<li>Arduino Duemilanove</li>
<li>Arduino IDE 0018</li>
<li>Servo Motor Futaba S3003</li>
<li>Microsoft Visual Studio 2008</li>
</ul>

<h3>More information:</h3>

Meu website: www.renatopeterman.com.br<br/>
Emotiv: www.emotiv.com<br/>
Arduino: www.arduino.cc<br/>

