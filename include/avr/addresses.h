#pragma once
namespace avr{
struct addr{
  enum{
    ddrb  = 0x24,
    portb = 0x25,
    ddrd  = 0x2b,
    portd = 0x2b
  }; //digital pins
  enum{
    tccr0a = 0x44,
    tccr0b = 0x45,
    tcnt0  = 0x46,
    ocr0a  = 0x47,
    ocr0b  = 0x48,
    tifr0  = 0x35,
    timsk0 = 0x6e,
  }; //Timer0

  enum{
    tccr2a = 0xb0,
    tccr2b,
    tcnt2,
    ocr2a,
    ocr2b,
    tifr2  = 0x37,
    timsk2 = 0x70,
  }; //Timer2

  enum{
    adcl   = 0x78,
    adch   = 0x79,
    adcsra = 0x7a,
    adcsrb = 0x7b,
    admux  = 0x7c,
    didr0  = 0x7e,
  }; //ADC


};//end of addr
namespace bits{
  struct adc{
    enum{
      adps0 = 0,
      adps1,
      adps2,
      adie,
      adif,
      adate,
      adsc,
      aden,
    }; //ADCSRA bits
  }; //end of adc

  struct timer0{
    enum{
      wgm0 = 0,
      wgm1,
      comb0 = 4,
      comb1,
      coma0,
      coma1
    }; //Bit positions for the TCCR0A register
    enum{
      cs0 = 0,
      cs1,
      cs2,
      wgm2,
      focb = 6,
      foca,
    }; //Bit positions for the TCCR0B register
    enum{
      tov,
      ocfa,
      ocfb
    }; //Bit posisitions for the TIFR0 register

  }; //end of timer0

}//end of bits
}//end of avr

