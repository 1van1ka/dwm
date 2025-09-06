void
monocle(Monitor *m)
{
	Client *c;

  int count = 0;
	int oh, ov, ih, iv;
  unsigned n;
	getgaps(m, &oh, &ov, &ih, &iv, &n);

  if (n - m->nmaster > 0) /* override layout symbol */
  	snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n - m->nmaster);

  for (c = nexttiled(m->clients); c; c = nexttiled(c->next), count++)
		resize(c, m->wx + ov, m->wy + oh, m->ww - 2 * c->bw - ov * 2, m->wh - 2 * c->bw - oh * 2, 0);
}
