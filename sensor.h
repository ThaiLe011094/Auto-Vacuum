#ifndef ULTRASOUND_H_
#define ULTRASOUND_H_


extern void Timer0IntHandler(void);
extern void DetectedISRE1(void);
extern void InitUltraSound1(void);
extern int DistanceReturn1(void);

extern void Timer1IntHandler(void);
extern void DetectedISRE2(void);
extern void InitUltraSound2(void);
extern int DistanceReturn2(void);

extern void Timer2IntHandler(void);
extern void DetectedISRE3(void);
extern void InitUltraSound3(void);
extern int DistanceReturn3(void);
#endif /* ULTRASOUND_H_ */
