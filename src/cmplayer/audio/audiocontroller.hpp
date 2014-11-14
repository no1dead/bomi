#ifndef AUDIOCONTROLLER_HPP
#define AUDIOCONTROLLER_HPP

struct af_instance;                     struct mp_audio;
struct af_cfg;                          struct af_info;
struct mp_chmap;                        struct AudioNormalizerOption;
class ChannelLayoutMap;
enum class ClippingMethod;              enum class ChannelLayout;

class AudioFormat {
public:
    auto operator == (const AudioFormat &rhs) const -> bool
    {
        return m_samplerate == rhs.m_samplerate && m_bitrate == rhs.m_bitrate
                && m_bits == rhs.m_bits && m_channels == rhs.m_channels
                && m_type == rhs.m_type;
    }
    auto operator != (const AudioFormat &rhs) const -> bool
        { return !operator == (rhs); }
    auto samplerate() const -> int { return m_samplerate; }
    auto bitrate() const -> int { return m_bitrate; }
    auto bits() const -> int { return m_bits; }
    auto type() const -> QString { return m_type; }
    auto channels() const -> QString { return m_channels; }
    auto nch() const -> int { return m_nch; }
private:
    friend class AudioController;
    int m_samplerate = 0, m_bitrate = 0, m_bits = 0, m_nch = 0;
    QString m_channels, m_type;
};

class AudioController : public QObject {
    Q_OBJECT
public:
    AudioController(QObject *parent = nullptr);
    ~AudioController();
    auto setNormalizerActivated(bool on) -> void;
    auto gain() const -> double;
    auto isTempoScalerActivated() const -> bool;
    auto isNormalizerActivated() const -> bool;
    auto setNormalizerOption(const AudioNormalizerOption &option) -> void;
    auto setClippingMethod(ClippingMethod method) -> void;
    auto setChannelLayoutMap(const ChannelLayoutMap &map) -> void;
    auto setOutputChannelLayout(ChannelLayout layout) -> void;
    auto chmap() const -> mp_chmap*;
    auto inputFormat() const -> AudioFormat;
    auto outputFormat() const -> AudioFormat;
    auto samplerate() const -> int;
signals:
    void inputFormatChanged();
    void outputFormatChanged();
    void samplerateChanged(int sr);
    void gainChanged(double gain);
private:
    auto reinitialize(mp_audio *data) -> int;
    static auto open(af_instance *af) -> int;
    static auto test(int fmt_in, int fmt_out) -> bool;
    static auto filter(af_instance *af, mp_audio *data, int flags) -> int;
    static auto uninit(af_instance *af) -> void;
    static auto control(af_instance *af, int cmd, void *arg) -> int;
    struct Data;
    Data *d;
    friend auto create_info() -> af_info;
};

#endif // AUDIOCONTROLLER_HPP
