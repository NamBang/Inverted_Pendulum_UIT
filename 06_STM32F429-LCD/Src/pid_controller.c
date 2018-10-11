
//*********************************************************************************
// Headers
//*********************************************************************************
#include "pid_controller.h"
#include <stdio.h>
#include "B_stm32f4_usart.h"
//*********************************************************************************
// Macros and Globals
//*********************************************************************************
#define CONSTRAIN(x,lower,upper)    ((x)<(lower)?(lower):((x)>(upper)?(upper):(x)))

//*********************************************************************************
// Functions
//*********************************************************************************
void PIDInit(PIDControl *pid, float kp, float ki, float kd, 
             float sampleTimeSeconds, float minOutput, float maxOutput, 
             PIDMode mode, PIDDirection controllerDirection)     	
{
    pid->controllerDirection = controllerDirection;
    pid->mode = mode;
    pid->iTerm = 0.0f;
    pid->input = 0.0f;
    pid->lastInput = 0.0f;
    pid->output = 0.0f;
    pid->setpoint = 0.0f;
    
    if(sampleTimeSeconds > 0.0f)
    {
        pid->sampleTime = sampleTimeSeconds;
    }
    else
    {
        // If the passed parameter was incorrect, set to 1 second
        pid->sampleTime = 1.0f;
    }
    
    PIDOutputLimitsSet(pid, minOutput, maxOutput);
    PIDTuningsSet(pid, kp, ki, kd);
}
        
bool
PIDCompute(PIDControl *pid) 
{
    float error, dInput;
//		char data_Buffer3[50];
//		char data_Buffer4[50];
//		char data_Buffer5[50];
//		char data_Buffer6[50];
    if(pid->mode == MANUAL)
    {
        return false;
    }
    
    // The classic PID error term
    error = (pid->setpoint) - (pid->input);
    
    // Compute the integral term separately ahead of time
    pid->iTerm += (pid->alteredKi) * error;
    
    // Constrain the integrator to make sure it does not exceed output bounds
    pid->iTerm = CONSTRAIN( (pid->iTerm), (pid->outMin), (pid->outMax) );
    
    // Take the "derivative on measurement" instead of "derivative on error"
    dInput = (pid->input) - (pid->lastInput);
    
    // Run all the terms together to get the overall output
    pid->output = (pid->alteredKp) * error + (pid->iTerm) - (pid->alteredKd) * dInput;
    
    // Bound the output
    pid->output = CONSTRAIN( (pid->output), (pid->outMin), (pid->outMax) );
    
    // Make the current input the former input
    pid->lastInput = pid->input;
//    sprintf(data_Buffer3, "erro: %0.3f\r\n", (float)(error));
//		USART_PutString(data_Buffer3);
//		
//		sprintf(data_Buffer4, "output: %0.3f\r\n", (float)( pid->output ));
//		USART_PutString(data_Buffer4);
//		
//		sprintf(data_Buffer5, "dInput: %0.3f\r\n", (float)( dInput ));
//		USART_PutString(data_Buffer5);
		
//		sprintf(data_Buffer6, "output: %0.3f\r\n", (float)( pid->output ));
//		USART_PutString(data_Buffer4);
		
		
    return true;
}
     
void 
PIDModeSet(PIDControl *pid, PIDMode mode)                                                                                                                                       
{
    // If the mode changed from MANUAL to AUTOMATIC
    if(pid->mode != mode && mode == AUTOMATIC)
    {
        // Initialize a few PID parameters to new values
        pid->iTerm = pid->output;
        pid->lastInput = pid->input;
        
        // Constrain the integrator to make sure it does not exceed output bounds
        pid->iTerm = CONSTRAIN( (pid->iTerm), (pid->outMin), (pid->outMax) );
    }
    
    pid->mode = mode;
}

void 
PIDOutputLimitsSet(PIDControl *pid, float min, float max) 							  							  
{
    // Check if the parameters are valid
    if(min >= max)
    {
        return;
    }
    
    // Save the parameters
    pid->outMin = min;
    pid->outMax = max;
    
    // If in automatic, apply the new constraints
    if(pid->mode == AUTOMATIC)
    {
        pid->output = CONSTRAIN(pid->output, min, max);
        pid->iTerm  = CONSTRAIN(pid->iTerm,  min, max);
    }
}

void 
PIDTuningsSet(PIDControl *pid, float kp, float ki, float kd)         	                                         
{
    // Check if the parameters are valid
    if(kp < 0.0f || ki < 0.0f || kd < 0.0f)
    {
        return;
    }
    
    // Save the parameters for displaying purposes
    pid->dispKp = kp;
    pid->dispKi = ki;
    pid->dispKd = kd;
    
    // Alter the parameters for PID
    pid->alteredKp = kp;
    pid->alteredKi = ki * pid->sampleTime;
    pid->alteredKd = kd / pid->sampleTime;
    
    // Apply reverse direction to the altered values if necessary
    if(pid->controllerDirection == REVERSE)
    {
        pid->alteredKp = -(pid->alteredKp);
        pid->alteredKi = -(pid->alteredKi);
        pid->alteredKd = -(pid->alteredKd);
    }
}

void 
PIDTuningKpSet(PIDControl *pid, float kp)
{
    PIDTuningsSet(pid, kp, pid->dispKi, pid->dispKd);
}

void 
PIDTuningKiSet(PIDControl *pid, float ki)
{
    PIDTuningsSet(pid, pid->dispKp, ki, pid->dispKd);
}

void 
PIDTuningKdSet(PIDControl *pid, float kd)
{
    PIDTuningsSet(pid, pid->dispKp, pid->dispKi, kd);
}

void 
PIDControllerDirectionSet(PIDControl *pid, PIDDirection controllerDirection)	  									  									  									  
{
    // If in automatic mode and the controller's sense of direction is reversed
    if(pid->mode == AUTOMATIC && controllerDirection == REVERSE)
    {
        // Reverse sense of direction of PID gain constants
        pid->alteredKp = -(pid->alteredKp);
        pid->alteredKi = -(pid->alteredKi);
        pid->alteredKd = -(pid->alteredKd);
    }
    
    pid->controllerDirection = controllerDirection;
}

void 
PIDSampleTimeSet(PIDControl *pid, float sampleTimeSeconds)                                                       									  									  									   
{
    float ratio;

    if(sampleTimeSeconds > 0.0f)
    {
        // Find the ratio of change and apply to the altered values
        ratio = sampleTimeSeconds / pid->sampleTime;
        pid->alteredKi *= ratio;
        pid->alteredKd /= ratio;
        
        // Save the new sampling time
        pid->sampleTime = sampleTimeSeconds;
    }
}

