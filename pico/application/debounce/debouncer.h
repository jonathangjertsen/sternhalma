#pragma once

class ActiveLowDebouncer
{
public:
    ActiveLowDebouncer(int n)
    {
        m_mask = (1 << n) - 1;
        m_target = (1 << n);
    }

    bool update(bool logic_level)
    {
        m_ring <<= 1;
        m_ring |= logic_level;
        if (m_ring & m_mask == m_target)
        {
            return true;
        }
        return false;
    }
private:
    uint32_t m_ring   = 0xffffffffU;
    uint32_t m_mask   = 0U;
    uint32_t m_target = 0U;
};
