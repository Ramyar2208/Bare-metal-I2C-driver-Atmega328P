/*  2-wire interface (TWI): 
 *  TWEN = 1 to enable I2C / TWI interface
 *  TWSTA = 1 - Eable Start condition
 *  TWSTO = 1 - Enable Stop condition
 *  TWDR - contains data to transfer  
 *  TWINT - Flag to be cleared by setting it to '1' after writing data into registers like TWDR, 
to continue TWI 
 *  TWSR - Status register - Indicates Start completed, ACK status,.,
*/

#define TSens A3
#define SDA A4                   // Built-in SDA I2C pin
#define SCL A5                   // Built-in SCL I2C pin
#define LCDaddr 0x27             // NXP PCF8574T slave address

#define I2c_write_LCDcmd(x) I2c_write(x, 0x0C, 0x08)
#define I2c_write_LCDdata(x) I2c_write(x, 0x0D, 0x09)

float deg, val;

void I2C_init()
{
  TWBR = 72;                                   // Bit rate
  TWSR = TWSR & (0xFC);                        // Prescale value - bit 0,1 (0, 0 = value of 1)
  TWCR |= (1 << TWEN); 
}

inline void Clear_Start()                                    
{
  TWCR ^= (1 << TWSTA);    
  delay(1);                                     // delay to give TWI time to finish current action         
}  

void I2c_datawrite(unsigned char d)            
{  
  TWDR = d;
  TWCR |= (1 << TWINT) ; 
  delay(1);                                     // delay to give TWI and LCD-en to finish current action 
  
/*if((TWSR & 0xFC) == 0x18) 
Serial.println("SLA+W sent and ACK received");
if((TWSR & 0xFC) == 0x28) 
Serial.println("Data byte sent and ACK received");*/
}

void I2c_write(unsigned char d, unsigned char a, unsigned char b)
{
  TWCR |= (1 << TWSTA) | (1 << TWINT) ;          // START condition     
  Clear_Start(); 
    
/*if((TWSR & 0xFC) == 0x08) 
Serial.println("Start Transmitted");*/                

  I2c_datawrite(LCDaddr << 1);                   // 0th bit will always be zero in ATMega328P              
  /* data write */ 
  I2c_datawrite(0x00);                        
  I2c_datawrite((d & 0xF0) | a);                                  
  I2c_datawrite((d & 0xF0) | b);               
  I2c_datawrite(((d & 0x0F) << 4) | a);        
  I2c_datawrite(((d & 0x0F) << 4) | b);        
  /* data write */
  
  TWCR |= (1 << TWSTO) | (1 << TWINT) ;          // STOP condition
  delay(10);  
}

 void LCD_init()
 {
  delay(20);                         
  I2c_write_LCDcmd(0x01);            
  I2c_write_LCDcmd(0x02);            
  I2c_write_LCDcmd(0x28);            
  I2c_write_LCDcmd(0x0C);            
  I2c_write_LCDcmd(0x80);      
 }

void inttochar(int d, char * q)                 
{
  q[4] = d % 10 + 48;
  q[3] = (d / 10) % 10 + 48;
  q[2] = '.';
  q[1] = (d / 100) % 10 + 48;
  q[0] = d / 1000 + 48;
}

void setup() {
//Serial.begin(9600);
  pinMode(TSens, INPUT);
  
  I2C_init();

  LCD_init();
}

void loop() {
  int i; char p[12] = {'T', 'E', 'M', 'P', ' ', '=', ' '};
  val = analogRead(TSens);           
  deg = (val * 500) / 1024; 

  I2c_write_LCDcmd(0x80);            
  for(i = 0; i <= 6; i ++)
    I2c_write_LCDdata(p[i]);
  inttochar(deg * 100, &p[7]);       
  for(; i < 12; i ++)
    I2c_write_LCDdata(p[i]);
 }
