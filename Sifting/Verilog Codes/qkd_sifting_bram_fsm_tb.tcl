# Optional: Waveform
if { [llength [get_objects]] > 0 } {
    add_wave /
    set_property needs_save false [current_wave_config]
}
run all
