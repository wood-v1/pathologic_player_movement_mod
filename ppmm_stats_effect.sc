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
		float dt = readSpeedDeltaFromVar();

		float sumSpeed = absf(CalcSumPlayerSpeed());
		
		if (timer >= 0.3) {

			if (sumSpeed > 50) {
				ModPropertyF("tiredness", dt, 0, 1);
				ReportTirednessChange(dt);
			}

			timer = 0;
		}

		return false;
	}

	float readSpeedDeltaFromVar() {
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
		return vSpeed.x + vSpeed.y + vSpeed.z;
	}

}
