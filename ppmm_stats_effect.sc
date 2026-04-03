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
		
		if (timer >= 0.3) {
			float sumSpeed = CalcSumPlayerSpeed();

			if (sumSpeed > 10) {
				float dt = ReadSpeedDeltaFromVariable();

				ModPropertyF("tiredness", dt, 0, 1);
				ReportTirednessChange(dt);
			}

			timer = 0;
		}

		return false;
	}

	float ReadSpeedDeltaFromVariable() {
		int dt;
		@GetVariable("ppmm_tiredness_delta", dt);

		if (!dt) {
			return 0.00030;
		}

		return dt * 0.00001;
	}

	float CalcSumPlayerSpeed() {
		Vector vSpeed;
		@GetSpeed(vSpeed);
		return absf(vSpeed.x) + absf(vSpeed.y) + absf(vSpeed.z);
	}

}
