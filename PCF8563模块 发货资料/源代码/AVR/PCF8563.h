/*********************��������***************************************/
void P8563_init() ;
void P8563_gettime();

/****************PCF8563 I2C�ӿڶ���*****************/
#define PCF8563T_SDA_ON      PORTA|=0x08       //i2c�����߸ߵ�ƽ
#define PCF8563T_SDA_OFF     PORTA&=~0x08      //i2c�����ߵ׵�ƽ
#define PCF8563T_SCL_ON      PORTA|=0x04      //i2c�����ߵ׵�ƽ
#define PCF8563T_SCL_OFF     PORTA&=~0x04      //i2c�����ߵ׵�ƽ
#define PCF8563T_DAT_PIN     PORTA|=0x08;DDRA&=~0x08       //i2c�����߶˿���Ϊ����
#define PCF8563T_DAT_POUT    DDRA|=0x08        //i2c�����߶˿���Ϊ���
#define PCF8563T_SDA_PIN     (PINA&0x08)        //i2c�����������ƽ

/****************PCF8563 �Ĵ����ӿڶ���*****************/
#define PCF8563T_GET_ADR      0xA3              //PCF8563��ͳһ��ַ
#define PCF8563T_SEND_ADR     0xA2              //PCF8563дͳһ��ַ
#define PCF8563T_SYS1         0x00              //PCF8563״̬�Ĵ�����ַ1
#define PCF8563T_SYS2         0x01              //PCF8563״̬�Ĵ�����ַ2
#define PCF8563T_S            0x02              //��
#define PCF8563T_MIN          0x03              //��
#define PCF8563T_OUR          0x04              //ʱ
#define PCF8563T_DAT          0x05              //��
#define PCF8563T_             0x06              //����
#define PCF8563T_MATH         0x07              //��
#define PCF8563T_YER          0x08              //��
#define PCF8563T_BJ1          0x09               //���ӱ���
#define PCF8563T_BJ2          0x0A               //Сʱ����
#define PCF8563T_BJ3          0x0B               //�ձ���
#define PCF8563T_BJ4          0x0C               //���ڱ���
#define PCF8563T_CLK          0x0D              //ʱ�Ӳ���
#define PCF8563T_CT           0x0E              //��ʱ��������ʽ
#define PCF8563T_COUNT        0x0F              //��ʱ��������ʽ
