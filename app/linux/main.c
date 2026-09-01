// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/fordlink.h"
#include "link-gtk-shell.h"
#include "link-gtk-widgets.h"
#include "link/workspace.h"
#include "link/obd2.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    bool connected, diagnostic_valid, diagnostic_ready;
    char adapter_identity[160];
    LinkDiagnosticFlow diagnostic;
    bool sample_valid[256];
    LinkObd2Sample samples[256];
} ProductContext;

static const char css[] =
"window { background:#090b0e; color:#eef1f4; }"
".link-connection-bar { background:#14181d; border:1px solid rgba(255,255,255,.14); }"
".link-link-button { background:#e8edf2; color:#111418; border-radius:10px; }"
".link-card { background:#12161b; border:1px solid rgba(255,255,255,.12); border-radius:16px; padding:18px; }"
".link-card-kicker { color:#9fa8b3; font-size:10px; font-weight:800; letter-spacing:1.5px; }"
".link-card-title { color:#f5f7f9; font-size:20px; font-weight:800; }"
".link-detail-label { color:#9fa8b3; }"
".link-detail-value { color:#f5f7f9; font-weight:700; }"
".link-card-note { color:#aeb6bf; }"
".link-status-chip { padding:6px 10px; border-radius:999px; border:1px solid rgba(255,255,255,.18); font-weight:700; }"
".state-success { color:#a8e0b9; border-color:#4f8c63; }"
".state-warning { color:#e0c486; border-color:#8b7444; }";

static const char *stage(const ProductContext *c){
    if(!c->connected) return "Not linked";
    if(!c->diagnostic_valid) return "Starting diagnostics";
    if(c->diagnostic.stage==LINK_DIAGNOSTIC_FLOW_FAILED) return "Diagnostic session failed";
    if(c->diagnostic_ready) return "Live diagnostics active";
    return link_diagnostic_flow_stage_name(c->diagnostic.stage);
}
static size_t pids(const LinkObd2PidSet *s){size_t n=0;for(unsigned p=1;p<256;p++)if(link_obd2_pid_set_contains(s,(uint8_t)p))n++;return n;}
static void dtcs(GtkWidget *card,const char *prefix,const LinkObd2DtcList *l){
    if(l->count==0){char k[48];snprintf(k,sizeof k,"%s faults",prefix);link_gtk_card_append_detail(card,k,"None reported");return;}
    for(size_t i=0;i<l->count;i++){char k[48];snprintf(k,sizeof k,"%s %zu",prefix,i+1);link_gtk_card_append_detail(card,k,l->entries[i].code);}
}
static void vehicle(GtkWidget *b,ProductContext *c){
    GtkWidget *v=link_gtk_card_new("VEHICLE","Ford diagnostic identity");
    GtkWidget *x=link_gtk_card_new("CONNECTION","LINK adapter and standards session");
    const char *vin=c->diagnostic_valid?link_diagnostic_flow_standard_vin(&c->diagnostic):NULL;
    link_gtk_card_append_detail(v,"Manufacturer","Ford");
    link_gtk_card_append_detail(v,"VIN",vin&&*vin?vin:"Waiting for SAE Mode 09 VIN");
    link_gtk_card_append_detail(v,"Standards core","LINK · SAE J1979 / OBD-II / ISO-TP / UDS");
    link_gtk_card_append_status(x,c->connected?"LINKED":"NOT LINKED",c->connected?"state-success":"state-warning");
    link_gtk_card_append_detail(x,"Adapter",c->adapter_identity[0]?c->adapter_identity:"Select an adapter above and press LINK UP");
    link_gtk_card_append_detail(x,"Diagnostic state",stage(c));
    link_gtk_card_append_note(x,"FORDLINK is GUI-first. LINK owns the standards transport and read-only diagnostic flow; manufacturer-specific claims remain evidence-gated.");
    gtk_box_append(GTK_BOX(b),v);gtk_box_append(GTK_BOX(b),x);
}
static void modules(GtkWidget *b,ProductContext *c){
    GtkWidget *card=link_gtk_card_new("MODULE RESPONDERS","Physical SAE responders seen during capability discovery");
    if(!c->diagnostic_valid||c->diagnostic.supported_pid_responders.count==0)link_gtk_card_append_status(card,"No responder inventory yet","state-warning");
    else for(size_t i=0;i<c->diagnostic.supported_pid_responders.count;i++){const LinkObd2ResponderPidSet *r=&c->diagnostic.supported_pid_responders.entries[i];char k[48],v[128];snprintf(k,sizeof k,"ECU %zu",i+1);snprintf(v,sizeof v,"%s CAN 0x%X · %zu advertised PIDs",r->extended_id?"29-bit":"11-bit",(unsigned)r->responder_id,pids(&r->supported_pids));link_gtk_card_append_detail(card,k,v);}
    link_gtk_card_append_note(card,"Desktop Research will extend this with bounded read-only manufacturer discovery; silence is never treated as an identity.");
    gtk_box_append(GTK_BOX(b),card);
}
static void faults(GtkWidget *b,ProductContext *c){
    GtkWidget *card=link_gtk_card_new("FAULT MEMORY","Stored, pending and permanent SAE OBD-II DTCs");
    if(!c->diagnostic_valid)link_gtk_card_append_status(card,"Not scanned","state-warning");
    else{dtcs(card,"Stored",&c->diagnostic.stored_dtcs);dtcs(card,"Pending",&c->diagnostic.pending_dtcs);dtcs(card,"Permanent",&c->diagnostic.permanent_dtcs);}
    gtk_box_append(GTK_BOX(b),card);
}
static void live(GtkWidget *b,ProductContext *c,const char *k,const char *t){
    GtkWidget *card=link_gtk_card_new(k,t);size_t n=0;
    for(unsigned p=1;p<256;p++){if(!c->diagnostic_valid||!link_obd2_pid_set_contains(&c->diagnostic.supported_pids,(uint8_t)p))continue;const LinkObd2PidDefinition *d=link_obd2_pid_definition(1,(uint8_t)p);char key[180],val[96];snprintf(key,sizeof key,"PID 0x%02X · %s",p,d&&d->name?d->name:link_obd2_pid_name((uint8_t)p));if(c->sample_valid[p]){const char *u=link_obd2_unit_name(c->samples[p].unit);snprintf(val,sizeof val,"%.3f%s%s",c->samples[p].value,u&&*u?" ":"",u?u:"");}else snprintf(val,sizeof val,"Supported · waiting for sample");link_gtk_card_append_detail(card,key,val);n++;}
    if(!n)link_gtk_card_append_status(card,"No live PID data yet","state-warning");gtk_box_append(GTK_BOX(b),card);
}
static void generic(GtkWidget *b,ProductContext *c,const char *k,const char *t,const char *note){GtkWidget *card=link_gtk_card_new(k,t);link_gtk_card_append_detail(card,"Connection",c->connected?"Linked":"Offline");link_gtk_card_append_detail(card,"Diagnostic state",stage(c));link_gtk_card_append_note(card,note);gtk_box_append(GTK_BOX(b),card);}
static void render(size_t s,GtkWidget *b,void *ctx){ProductContext *c=ctx;switch((LinkWorkspaceSection)s){case LINK_WORKSPACE_VEHICLE:vehicle(b,c);break;case LINK_WORKSPACE_MODULES:modules(b,c);break;case LINK_WORKSPACE_FAULTS:faults(b,c);break;case LINK_WORKSPACE_LIVE_DATA:live(b,c,"LIVE DATA","Supported SAE parameters");break;case LINK_WORKSPACE_TABLE:live(b,c,"TABLE","Dense live parameter table");break;case LINK_WORKSPACE_DASHBOARD:generic(b,c,"DASHBOARD","Diagnostic overview","Dashboard tiles use the same LINK live samples; there is no CLI product mode.");break;case LINK_WORKSPACE_GRAPHS:generic(b,c,"GRAPHS","Time-series workspace","Graphs consume recorded LINK telemetry and will expand with verified manufacturer data.");break;case LINK_WORKSPACE_LOG:generic(b,c,"LOG","Diagnostic evidence","Use SAVE SESSION to preserve the raw investigation record.");break;case LINK_WORKSPACE_SETTINGS:generic(b,c,"SETTINGS","FORDLINK preferences","Adapter and diagnostic behaviour are shared through LINK.");break;default:break;}}
static void conn(LinkTransport *t,bool connected,const char *id,void *ctx){ProductContext *c=ctx;(void)t;c->connected=connected;snprintf(c->adapter_identity,sizeof c->adapter_identity,"%s",id?id:"");if(!connected){c->diagnostic_valid=false;c->diagnostic_ready=false;memset(c->sample_valid,0,sizeof c->sample_valid);}}
static void diag(const LinkDiagnosticFlow *f,const LinkDiagnosticFlowEvent *e,bool active,bool ready,void *ctx){ProductContext *c=ctx;(void)active;if(f){c->diagnostic=*f;c->diagnostic_valid=true;}c->diagnostic_ready=ready;if(e&&e->kind==LINK_DIAGNOSTIC_FLOW_EVENT_LIVE_SAMPLE){uint8_t p=e->sample.pid;c->samples[p]=e->sample;c->sample_valid[p]=true;}}
int main(int argc,char **argv){ProductContext c={0};LinkGtkShellDescriptor d={0};d.app_id="com.github.InfiltratorProjects.FORDLINK";d.window_title="FORDLINK · Ford Diagnostics";d.brand_name="FORDLINK";d.brand_subtitle="Ford · LINK standards diagnostics";d.version=fordlink_version();d.css=css;d.render_section=render;d.connection_changed=conn;d.diagnostic_changed=diag;d.use_client_side_titlebar=true;d.adapter_combo_width=320;d.context=&c;return link_gtk_shell_run(argc,argv,&d);}
