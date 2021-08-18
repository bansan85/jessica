import { Pipe, PipeTransform } from '@angular/core';

@Pipe({
  name: 'translate'
})
export class TranslateMockPipe implements PipeTransform {
  public name = 'translate';

  public transform(query: string): string {
    return query;
  }
}
