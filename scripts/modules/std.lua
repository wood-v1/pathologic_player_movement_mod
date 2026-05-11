module std do
  function absf(value: float) -> float
    if value < 0 then
      return -value
    end

    return value
  end

  function clampf(value: float, minValue: float, maxValue: float) -> float
    if value < minValue then
      return minValue
    end

    if value > maxValue then
      return maxValue
    end

    return value
  end

  function ModPropertyF(propName: string, amount: float, minValue: float, maxValue: float) -> void
    local hasProperty: bool
    native.HasProperty(propName, hasProperty)
    if not hasProperty then
      return
    end

    local value: float
    native.GetProperty(propName, value)

    local clampedValue: float = clampf(value + amount, minValue, maxValue)
    native.SetProperty(propName, clampedValue)
  end

  function ReportTirednessChange(amount: float) -> void
    local data: object
    native.CreateFloatVector(data)
    data.add(amount)
    native.SendWorldWndMessage(11, data)
  end
end
