#ifndef SCHEDULER_H
#define SCHEDULER_H
void timerSetting(int numMilliSeconds);
void aging(struct list_head *head);
void scheduler(void);
#endif /*SCHEDULER_H*/