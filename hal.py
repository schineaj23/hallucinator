from link_wrapper import Link

# Create a Link instance at 120 BPM
link = Link(120.0)

# Enable Link
link.enable(True)

# Set up a callback for when peers join/leave
def on_peers_changed(num_peers):
    print(f"Number of peers changed: {num_peers}")

link.set_num_peers_callback(on_peers_changed)

while True:
    # Capture the current state
    state = link.capture_app_session_state()
    print(state)

    # Get the current tempo
    current_tempo = state.tempo()