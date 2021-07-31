#include <light.h>
#include <packet.h>

#include <stdio.h>
#include <assert.h>

int main()
{
    uint8_t buf[64];
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);

    // Test Player Light
    joycon_packet_player_light_disable(buf, 0);
    assert(hdr->command == Subcommand);
    assert(pkt->subcommand == SetPlayerLight);
    assert(pkt->args.arg1 == 0);

    joycon_packet_player_light_enable(buf, 0, PLAYER_LIGHT_KEEP_2);
    assert(hdr->command == Subcommand);
    assert(pkt->subcommand == SetPlayerLight);
    assert(pkt->args.arg1 == PLAYER_LIGHT_KEEP_2);

    // TODO: Test Home light

    return 0;
}