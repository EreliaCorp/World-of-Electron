#pragma once

#include "widget/woe_abstract_manager.h"
#include "widget/woe_iwidget.h"
#include "woe_network.h"
#include "structure/woe_entity.h"

class Entity_manager : public virtual Abstract_manager, public virtual IWidget
{
private:
	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();

	void _upload_entity(Message& p_msg, Entity* p_entity);
	Entity* _download_entity(Message& p_msg);

	void _initiate_server();
	void _initiate_client();

	void _push_entity_update();
	void _pull_entity_update(Message& p_msg);
	void _push_entity_data(Connection* p_client, Message& p_msg);
	void _pull_entity_data(Message& p_msg);

	jgl::Timer _entity_updater_timer = jgl::Timer(8);
	jgl::Map<jgl::Long, jgl::Bool> _asked_entities;
	jgl::Ulong _last_tick;

public:
	Entity_manager(jgl::Widget* p_parent);
};