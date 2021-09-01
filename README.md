# XM
Extremely fast Finite State Machine pure C implementation with events subsystem.

## Features
* events handling with events queue
* configurable static or dynamic allocation
* static states and events descriptors
* state transitions, events and process callbacks
* init and finish conditions
* no external dependencies
* object oriented architecture
* thread safe (optional mutex interface)
* dedicated for embedded systems
* multi-platform architecture
* nice design and internal architecture
* pure C source code

## Build

Build, test and run examples:

```sh
cmake -B build .
cd build
make
make test
./bin/event_1
./bin/event_1_mutex
./bin/state_1
./bin/state_1_static
```

## API

```c
typedef int (*xm_state_event_cb)(struct xm_object *self, xm_event_id_t id, void *arg);
typedef int (*xm_state_transition_cb)(struct xm_object *self, bool enter);
typedef int (*xm_state_process_cb)(struct xm_object *self);

xm_status_t     xm_init(struct xm_object *self, const struct xm_object_descriptor *desc);
xm_status_t     xm_deinit(struct xm_object *self);
xm_status_t     xm_service(struct xm_object *self);
xm_status_t     xm_event_trigger(struct xm_object *self, xm_event_id_t id, void *arg);
xm_status_t     xm_state_request(struct xm_object *self, xm_state_id_t id);
xm_state_id_t   xm_state_get_current(struct xm_object *self);
xm_status_t     xm_finish(struct xm_object *self);
bool            xm_is_finish(struct xm_object *self);

struct xm_object*       xm_new(const struct xm_object_descriptor *desc);
void                    xm_delete(struct xm_object* self);
```

## Usage

Declare states:
```c
typedef enum {
        STATE_1,
        STATE_2,
        STATE_3,
} states_t;

static int state1_event_cb(struct xm_object *self, xm_event_id_t id, void *arg);
static int state2_transition_cb(struct xm_object *self, bool enter);
static int state3_process_cb(struct xm_object *self);

static const struct xm_state_descriptor g_states_desc[] = {
        {
                .name = "STATE_1",
                .id = STATE_1,
                .event_cb = state1_event_cb,
        },
        {
                .name = "STATE_2",
                .id = STATE_2,
                .transition_cb = state2_transition_cb,
        },
        {
                .name = "STATE_3",
                .id = STATE_3,
                .process_cb = state3_process_cb,
        },
        {0}
};
```

Declare events:
```c
typedef enum {
        EVENT_1,
        EVENT_2,
} events_t;

static const struct xm_event_descriptor g_events_desc[] = {
        {
                .name = "EVENT_1",
                .id = EVENT_1,
        },
        {
                .name = "EVENT_2",
                .id = EVENT_2,
        },
        {0}
};
```

Instance and run:
```c
static const struct xm_object_descriptor g_fsm_desc = {
        .states = g_states_desc,
        .events = g_events_desc,
        .init_state_id = STATE_1,
        .name = "main"
};

struct xm_object g_fsm;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        xm_init(&g_fsm, &g_fsm_desc);
        while (xm_is_finish(&g_fsm) == false)
                xm_service(&g_fsm);
        xm_deinit(&g_fsm);

        return 0;
}
```

Example multithread flow:
```sh
xm: main: [I]: request state: <STATE_COUNT>
xm: main: [I]: init
xm: main: [I]: enter: <STATE_COUNT>
xm: main: [I]: event triggered: [EVENT_1]
xm: main: [I]: event processed: [EVENT_1]
xm: main: [I]: event triggered: [EVENT_2]
xm: main: [I]: event processed: [EVENT_2]
xm: main: [I]: event triggered: [EVENT_1]
xm: main: [I]: event processed: [EVENT_1]
xm: main: [I]: event triggered: [EVENT_2]
xm: main: [I]: event processed: [EVENT_2]
xm: main: [I]: event triggered: [EVENT_1]
xm: main: [I]: event processed: [EVENT_1]
xm: main: [I]: request state: <STATE_FINAL>
xm: main: [I]: exit: <STATE_COUNT>
xm: main: [I]: enter: <STATE_FINAL>
xm: main: [I]: event triggered: [EVENT_FINISH]
xm: main: [I]: event processed: [EVENT_FINISH]
xm: main: [I]: exit: <STATE_FINAL>
xm: main: [I]: finish
```
