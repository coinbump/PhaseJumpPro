using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum RedGreenState {
	Invalid, Green, Red
}

public class RedGreenStateMachine : PJ.GenericStateMachine<RedGreenState> {
}
