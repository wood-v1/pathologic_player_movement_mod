task TimedLoop do
  var bool m_bStop

  function init(fWorkTime: float) -> void
    local fStartTime: float
    native.GetGameTime(fStartTime)

    local fEndTime: float = fStartTime + fWorkTime
    local fLastTime: float = fStartTime

    while true do
      native.sync()

      if m_bStop then
        return
      end

      local fTime: float
      native.GetGameTime(fTime)

      if fTime <= fLastTime then
        continue
      end

      if fTime >= fEndTime then
        local timeoutDelta: float = fEndTime - fLastTime
        Tick(timeoutDelta)
        return
      end

      local regularDelta: float = fTime - fLastTime
      if Tick(regularDelta) then
        return
      end

      fLastTime = fTime
    end
  end

  function Stop() -> void
    m_bStop = true
  end
end
