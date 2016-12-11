/*  
 * Controle de nível de reservatório utilizando Arduino
 * 
 * Criado para demonstração da utilização do Arduino em vídeos do canal 
 * Mundo da Elétrica no Youtube.
 *  
 * Este software tem finalidade didática e demonstrativa, e portanto 
 * aspectos relacionados a eficiência não são o objetivo.
 *
 * Conheça o canal Mundo da Elétrica utilizando o link abaixo:
 * http://www.youtube.com/subscription_center?add_user=MundoDaEletrica
 *  
 * Autor: Alex Teixeira Benfica 
 * https://www.linkedin.com/in/alexbenfica   
 *  
 */


#define LED_AMARELO A1
#define LED_VERDE   A2

#define APAGADO LOW
#define ACESO   HIGH

#define BUZZER 5

#define SENSOR_MIN 7
#define SENSOR_MAX 8

#define BOMBA 9
#define LIGADA LOW
#define DESLIGADA HIGH

// Estados do reservatorio
#define VAZIO 0
#define INTERMEDIARIO 1
#define CHEIO 2
#define DEFEITO 3


unsigned char novo(unsigned char novo_estado);


// indica transição de estado do reservatório
boolean transicao = true;

// estado do reservatorio
unsigned int reservatorio = novo(VAZIO);



// funçao de setup chamada apenas 1 vez na inicializacao do programa.
void setup() {
  // inicializa os led sinalizadores
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT); 
  
  // inicializa o pino da bomba como saída
  pinMode(BOMBA, OUTPUT); 

  // inicializa o pino do buzzer
  pinMode(BUZZER, OUTPUT);   

  // liga os resistores de pull up dos pinos dos sensores
  pinMode(SENSOR_MAX, INPUT_PULLUP);
  pinMode(SENSOR_MIN, INPUT_PULLUP);   
}




// função que executa continuamente a partir da inicialização do programa.
void loop() {    

  // le os sensores máximo e mínimo continuamente 
  boolean min,max;
  min = sensor_min();
  max = sensor_max();  

  // aciona os leds indicativos de cada um dos sensores
  led_amarelo(!min);
  led_verde(!max);   

  // monitora o estado de defeito
  if(!max && min){
    reservatorio = novo(DEFEITO);        
  }
 
  // trata cada um dos estados do reservatório!
  switch(reservatorio){
    case VAZIO:
      if(houve_transicao()){
        bomba(LIGADA);
      }          
      if(min){
        reservatorio = novo(INTERMEDIARIO);
      }    
    break;
    
    case INTERMEDIARIO:
      if(houve_transicao()){
        // nada muda ao entrar ou sair deste estado
      }
      if(max){
        reservatorio = novo(CHEIO);
      }              
      if(!min){
        reservatorio = novo(VAZIO);
      }             
    break;
    
    case CHEIO:
      if(houve_transicao()){
        bomba(DESLIGADA);
      }
      if(!max){
        reservatorio = novo(INTERMEDIARIO);
      }                  
    break;
    
    case DEFEITO:    
      if(houve_transicao()){
        bomba(DESLIGADA);        
        buzzer(50);
      }
      if(max && min){
        reservatorio = novo(VAZIO);
        buzzer(0);                  
      }
    break;
  }    
}



// informa se houve transição de estado e ajusta a variavel de transicao
unsigned char houve_transicao(){
  if(transicao){
    transicao = false;
    return true;
  }
  return false;
}

unsigned char novo(unsigned char novo_estado){
  transicao = true;
  return novo_estado;
}

// liga o buzzer 
void buzzer(unsigned char pwm){
  analogWrite(BUZZER, pwm);  
}

// acende ou apaga o led amarelo
void led_amarelo(boolean estado){
  digitalWrite(LED_AMARELO, estado);
}

// acende ou apaga o led verde
void led_verde(boolean estado){
  digitalWrite(LED_VERDE, estado);
}

// lê a posição do sensor de níve máximo
boolean sensor_max(){
  return (digitalRead(SENSOR_MAX) == LOW);
}

// lê a posição do sensor de níve mínimo
boolean sensor_min(){
  return (digitalRead(SENSOR_MIN) == LOW);
}

// aciona a bomba que enche o reservatório
void bomba(boolean estado){
    digitalWrite(BOMBA, estado);
}
