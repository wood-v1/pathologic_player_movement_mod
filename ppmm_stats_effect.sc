include std.sci
include item_slow.sci

maintask TEffect: TSlow
{
	var float timer;

	void init(void) {
		super.init(12);
	}

	void OnTrigger(string name) {
		if (name == "ppmm_stats_effect_stop") {
			Stop();
		}
	}
	
	bool Tick(float fDeltaTime) {
		timer += fDeltaTime * 500;
		float dt = ReadSpeedDeltaFromVar();

		float sumSpeed = CalcSumPlayerSpeed();
		
		if (timer >= 0.3) {

			if (sumSpeed > 10) {
				ModPropertyF("tiredness", dt, 0, 1);
				ReportTirednessChange(dt);
			}

			timer = 0;
		}

		return false;
	}

	float ReadSpeedDeltaFromVar() {
		int dt;
		@GetVariable("ppmm_tiredness_delta", dt);
		float dtf;

		if (!dt) {
			return 0.00015;
		}

		return dt * 0.00001;
	}

	float CalcSumPlayerSpeed() {
		Vector vSpeed;
		@GetSpeed(vSpeed);
		return absf(vSpeed.x) + absf(vSpeed.y) + absf(vSpeed.z);
	}

}
