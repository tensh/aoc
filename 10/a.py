
from functools import partial
import re

class Game:
  def __init__(self):
    self.bots = {}
    self.outputs = {}

  def _create_or_get(self, bot):
    if not bot in self.bots:
      self.bots[bot] = Bot(bot)
    return self.bots[bot]

  def _add_output(self, output, value):
    if not output in self.outputs:
      self.outputs[output] = []
    self.outputs[output].append(value)

  def handle_value_cmd(self, cmd):
    bot = self._create_or_get(cmd.bot)
    bot.receive(cmd.value)

  def handle_bot_cmd(self, cmd):
    source_bot = self._create_or_get(cmd.source_bot)
    def callback(values):
      self._run_bot_cmd(cmd, values)
    source_bot.command(callback)

  def _run_bot_cmd(self, cmd, values):
    def give_target(target_type, target_id, value):
      if target_type == 'bot':
        bot = self._create_or_get(target_id)
        bot.receive(value)
      else:
        self._add_output(target_id, value)

    give_target(cmd.low_receiver, cmd.low_id, values[0])
    give_target(cmd.high_receiver, cmd.high_id, values[1])

  def solution_to_b(self):
    return self.outputs[0][0] * self.outputs[1][0] * self.outputs[2][0]

  def print_contents(self):
    print "Bots:"
    for key, value in self.bots.iteritems():
      print "%s: %s" % (key, value.to_str())
    print "Outputs:"
    print self.outputs

class Bot:
  def __init__(self, bot_id):
    self.bot_id = bot_id
    self.values = []
    self.queue = []

  def _process_queue(self):
    if self._hands_full() and len(self.queue) > 0:
      callback = self.queue.pop()
      callback(self.give())

  def command(self, callback):
    self.queue.insert(0, callback)
    self._process_queue()

  def carries_solution(self):
    return 17 in self.values and 61 in self.values

  def _hands_full(self):
    return len(self.values) == 2

  def receive(self, v):
    assert(not self._hands_full())
    self.values.append(v)
    self.values.sort()
    if self.carries_solution():
      print "Found solution to exercise A with bot #%d" % self.bot_id
    self._process_queue()

  def give(self):
    values = self.values
    self.values = []
    return values

  def to_str(self):
    return str(self.values)

class ValueCommand:
  def __init__(self, value, bot):
    self.value = int(value)
    self.bot = int(bot)

class BotCommand:
  def __init__(self, source_bot, low_receiver, low_id, high_receiver, high_id):
    self.source_bot = int(source_bot)
    self.low_receiver = low_receiver
    self.low_id = int(low_id)
    self.high_receiver = high_receiver
    self.high_id = int(high_id)

def parse_value_cmd(line):
  m = re.match('value (\d+) goes to bot (\d+)', line)
  return None if not m else ValueCommand(*m.groups())

def parse_bot_cmd(line):
  m = re.match('bot (\d+) gives low to (bot|output) (\d+) and high to (bot|output) (\d+)', line)
  return None if not m else BotCommand(*m.groups())

def handle_line(game, line):
  #print "Matching '%s' ..." % line
  value_cmd = parse_value_cmd(line)
  bot_cmd = None if value_cmd else parse_bot_cmd(line)
  if value_cmd:
    game.handle_value_cmd(value_cmd)
  elif bot_cmd:
    game.handle_bot_cmd(bot_cmd)
  else:
    print "no match"


def main():
  with open('input') as f:
    game = Game()
    for line in f:
      handle_line(game, line.rstrip())
    #game.print_contents()
    print "Solution to exercise B is %d" % game.solution_to_b()

if __name__ == '__main__':
  main()
