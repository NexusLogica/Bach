/**********************************************************************

File     : RandomNormal.cpp
Project  : PlaySpace Game Engine Class Library
Purpose  : Source file for a normal distribution random number generator.
Revisions: Original definition by Lawrence Gunn.
           2012/08/01

Notes    : The algorithm and code is taken from http://www.jstatsoft.org/v05/i08 by George Marsaglia and Wai Wan Tsang,
           "The Ziggurat Method for Generating Random Variables" in Journal of Statistical Software, Oct 2000.

           To use this, pass in an average and standard deviation. Almost all values out will be within +/- 2 times
           the standard deviation. So to get a value between 800 and 1200, pass in an average of 1000 and std dev of 100.

Copyright (c) 2012 by Lawrence Gunn
All Rights Reserved.

*/

#include "RandomNormal.h"
#include "RandomGenerator.h"
#include <math.h>

using namespace Bach;
using namespace boost;

#define SHR3 (jz=jsr, jsr^=(jsr<<13), jsr^=(jsr>>17), jsr^=(jsr<<5),jz+jsr)
#define UNI (0.5f + (signed) SHR3*0.2328306e-9f)
#define IUNI SHR3

#define REXP (jz = SHR3, iz = jz&255, (jz <ke[iz])? jz*we[iz] : ExponentialGen())

    //********************
    //* PS::RandomNormal *
    //********************

RandomNormal::RandomNormal() : m_jsrInitial(123456789) {
  jsr = m_jsrInitial;
  m_seedObject = RandomGenerator::GetDefaultRandomGenerator();
}

RandomNormal::~RandomNormal() {
}

shared_ptr<RandomNormal> RandomNormal::CreateInstance() {
  shared_ptr<RandomNormal> instance(new RandomNormal());
  instance->m_currentSeed = (unsigned long) instance->GetSeedObject()->GetSeed();
  instance->ZigguratInitialize(instance->m_currentSeed);
  return instance;
}

Real RandomNormal::GetRandomNum(Real average, Real deviation) {
  if(m_seedObject->GetSeed() != m_currentSeed) {
    m_currentSeed = (unsigned long) m_seedObject->GetSeed();
    ZigguratInitialize(m_currentSeed);
  }

  hz = SHR3;
  iz = hz&127;
  Real value = (fabs((Real) hz) < (Real) kn[iz] ? hz*wn[iz] : NormalGen());

  value *= deviation;
  value += average;
  return value;
}

// The ziggurat method for RNOR and REXP
// Combine the code below with the main program in which you want
// normal or exponential variates.   Then use of RNOR in any expression
// will provide a standard normal variate with mean zero, variance 1,
// while use of REXP in any expression will provide an exponential variate
// with density exp(-x),x>0.
// Before using RNOR or REXP in your main, insert a command such as
// zigset(86947731 );
// with your own choice of seed value>0, rather than 86947731.
// (If you do not invoke zigset(...) you will get all zeros for RNOR and REXP.)
// For details of the method, see Marsaglia and Tsang, "The ziggurat method
// for generating random variables", Journ. Statistical Software.

/* nfix() generates variates from the residue when rejection in RNOR occurs. */

Real RandomNormal::NormalGen() {
  const Real r = 3.442620f;     // The start of the right tail
  static Real x, y;
  for(;;) {
    x = hz*wn[iz];      // iz==0, handles the base strip
    if(iz == 0) {
      do {
        x = -log(UNI)*0.2904764f; // .2904764 is 1/r
        y = -log(UNI);
      } while(y+y<x*x);
      return (hz>0)? r+x : -r-x;
    }

    // iz>0, handle the wedges of other strips
    if( fn[iz]+UNI*(fn[iz-1]-fn[iz]) < exp(-.5*x*x) ) {
      return x;
    }

    // initiate, try to exit for(;;) for loop
    hz = SHR3;
    iz = hz&127;
    if(fabs((Real) hz)< (Real) kn[iz]) {
      return hz*wn[iz];
    }
  }
}

/* efix() generates variates from the residue when rejection in REXP occurs. */
Real RandomNormal::ExponentialGen() {
  Real x;
  for(;;) {
    if(iz == 0) {
      return 7.69711f-log(UNI);  // iz==0
    }
    x = jz*we[iz];
    if(fe[iz]+UNI*(fe[iz-1]-fe[iz]) < exp(-x)) {
      return x;
    }

      /* initiate, try to exit for(;;) loop */
   jz = SHR3;
   iz = jz&255;
   if(jz<ke[iz]) {
     return jz*we[iz];
   }
  }
}

// Sets the seed and creates the tables
void RandomNormal::ZigguratInitialize(unsigned long jsrSeed) {
  jsr = m_jsrInitial;
  const Real m1 = 2147483648.0f;
  const Real m2 = 4294967296.0f;
  Real dn = 3.442619855899f;
  Real tn = dn;
  Real vn = 9.91256303526217e-3f;
  Real q;
  Real de=7.697117470131487f;
  Real te = de;
  Real ve = 3.949659822581572e-3f;
  int i;
  jsr ^= jsrSeed;

  // Set up tables for RNOR
  q = vn/exp(-0.5f*dn*dn);
  kn[0] = (unsigned long) ((dn/q)*m1);
  kn[1] = 0;

  wn[0] = q/m1;
  wn[127] = dn/m1;

  fn[0] = 1.0f;
  fn[127] = exp(-0.5f*dn*dn);

  for(i=126; i>=1; i--) {
    dn = sqrt(-2.0f*log(vn/dn+exp(-0.5f*dn*dn)));
    kn[i+1] = (unsigned long) ((dn/tn)*m1);
    tn = dn;
    fn[i] = exp(-0.5f*dn*dn);
    wn[i] = dn/m1;
  }

  // Set up tables for REXP
  q = ve/exp(-de);
  ke[0] = (unsigned long) ((de/q)*m2);
  ke[1] = 0;

  we[0] = q/m2;
  we[255] = de/m2;

  fe[0]=1.0;
  fe[255]=exp(-de);

  for(i=254; i>=1; i--) {
    de = -log(ve/de+exp(-de));
    ke[i+1] = (unsigned long) ((de/te)*m2);
    te = de;
    fe[i] = exp(-de);
    we[i] = de/m2;
  }
}

void RandomNormal::Test() {
  for(int i=0; i<3000; i++) {
    LogPlain(L"%f", GetRandomNum(5.0f, 2.0f));
  }
}
