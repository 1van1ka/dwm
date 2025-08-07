void
monocle(Monitor *m)
{
	Client *c;

	int oh, ov, ih, iv;
  unsigned n;
	getgaps(m, &oh, &ov, &ih, &iv, &n);
  oh *= smartgaps_fact;
  ov *= smartgaps_fact;
  ih *= smartgaps_fact;
  iv *= smartgaps_fact;

	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx + ov, m->wy + oh, m->ww - 2 * c->bw - ov * 2, m->wh - 2 * c->bw - oh * 2, 0);
}

