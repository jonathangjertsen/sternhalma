#pragma once

class ActiveLowDebouncer
{
public:
    explicit ActiveLowDebouncer(int n = 3)
    {
        m_mask = (1 << n) - 1;
        m_target = (1 << (n - 1));
    }

    bool update(bool raw_pressed)
    {
        m_ring <<= 1;
        m_ring |= !raw_pressed;
        if ((m_ring & m_mask) == m_target)
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
