import "std"
import "timed_loop"

maintask TEffect extends TimedLoop do
  local timer: float

  function init() -> void
    super.init(12)
  end

  function OnTrigger(name: string) -> void
    if name == "ppmm_stats_effect_stop" then
      Stop()
    end
  end

  function Tick(fDeltaTime: float) -> bool
    timer = timer + fDeltaTime * 500

    if timer >= 0.3 then
      ApplyTirednessEffect()
      timer = 0
    end

    return false
  end

  function ApplyTirednessEffect() -> void
    local hasTiredness: bool
    native.HasProperty("tiredness", hasTiredness)
    if not hasTiredness then
      return
    end

    if CalcSumPlayerSpeed() <= 10 then
      return
    end

    local dt: float = ReadSpeedDeltaFromVariable()
    ModPropertyF("tiredness", dt, 0, 1)
    ReportTirednessChange(dt)
  end

  function ReadSpeedDeltaFromVariable() -> float
    local dt: int
    native.GetVariable("ppmm_tiredness_delta", dt)

    if dt <= 0 then
      return 0.00030
    end

    return dt * 0.00001
  end

  function CalcSumPlayerSpeed() -> float
    local speed: Vector
    native.GetSpeed(speed)

    local speedX: float = absf(index(speed, 0))
    local speedY: float = absf(index(speed, 1))
    local speedZ: float = absf(index(speed, 2))

    return speedX + speedY + speedZ
  end

end
